//
// Created by fabien sanglard on 2017-11-23.
//

#include <assert.h>
#include <iostream>
#include "Map.h"
#include "SVGRenderer.h"

struct NodeEntry {
    int16_t xstart, ystart;
    int16_t dx, dy;
    uint8_t rightAABB[8];
    uint8_t leftAABB[8];

    int16_t leftChildId; // Either a nodeId or a subSectorId (leaf)
    int16_t rightChildId;// Either a nodeId or a subSectorId (leaf)
};

struct VertexEntry {
    int16_t x;
    int16_t y;
};

struct LineDefEntry {
    uint16_t fromId; // vertex id
    uint16_t toId;   // vertex id
    int16_t flags;
    int16_t types;
    int16_t tags;
    int16_t rightSideDef;
    int16_t leftSideDef;
};

struct SideDefEntry {
    int16_t dx;
    int16_t dy;
    char upper[8];
    char middle[8];
    char lower[8];
    int16_t sectorId;
};

struct SegEntry {
    uint16_t vertexStartId;
    uint16_t vertexEndId;
    int16_t angle;
    int16_t lineDefId;
    int16_t direction;
    int16_t offset;
};

struct SubSectorEntry {
    uint16_t numSegs;
    uint16_t segStartId;
};

void Map::parseNodes(Lump &lump) {
    assert(!strncmp(lump.name().c_str(), "NODES", 5));
    size_t numNodes = lump.size() / 28;
    NodeEntry *entry = reinterpret_cast<NodeEntry *>(lump.payload());
    for (int i = 0; i < numNodes; i++) {
        Node node = {entry->xstart,
                     entry->ystart,
                     entry->xstart + entry->dx,
                     entry->ystart + entry->dy,
                     entry->leftChildId,
                     entry->rightChildId
        };
        mNodes.push_back(node);
        entry++;
    }
}

void Map::parseLineDefs(Lump &lump) {
    assert(!strncmp(lump.name().c_str(), "LINEDEFS", 8));
    size_t numLinesDefs = lump.size() / 14;
    LineDefEntry *entry = reinterpret_cast<LineDefEntry *>(lump.payload());
    for (int i = 0; i < numLinesDefs; i++) {
        Point start = mVertices.at(entry->fromId);
        Point end = mVertices.at(entry->toId);
        mLinesDegs.push_back({start, end});
        entry++;
    }
}

void Map::parseVertices(Lump &lump) {
    assert(!strncmp(lump.name().c_str(), "VERTEXES", 8));
    size_t numVertices = lump.size() / 4;
    VertexEntry *vertex = reinterpret_cast<VertexEntry *>(lump.payload());
    for (int i = 0; i < numVertices; i++) {
        mVertices.push_back({vertex->x, vertex->y});
        vertex++;
    }

    // Compute bounding box
    Point min = {INT16_MAX, INT16_MAX};
    Point max = {INT16_MIN, INT16_MIN};
    for (Point point: mVertices) {
        if (point.x < min.x)
            min.x = point.x;
        if (point.y < min.y)
            min.y = point.y;

        if (point.x > max.x)
            max.x = point.x;
        if (point.y > max.y)
            max.y = point.y;
    }
    mBoundingBox = AABB(min, max);
}

void Map::parseSideDefs(Lump &lump) {
    assert(!strncmp(lump.name().c_str(), "SIDEDEFS", 8));

}

void Map::parseSubSectors(Lump &lump) {
    assert(!strncmp(lump.name().c_str(), "SSECTORS", 8));
    size_t numSubSectors = lump.size() / 4;
    SubSectorEntry *entry = reinterpret_cast<SubSectorEntry *>(lump.payload());
    for (int i = 0; i < numSubSectors; i++) {
        SubSector *subSector = new SubSector();
        for (int j = 0; j < entry->numSegs; j++) {
            subSector->addSeg(mSegs.at(entry->segStartId + j));
        }
        mSubSectors.push_back(subSector);
        entry++;
    }
}

void Map::parseSegs(Lump &lump) {
    assert(!strncmp(lump.name().c_str(), "SEGS", 4));
    size_t numSegs = lump.size() / 12;
    SegEntry *entry = reinterpret_cast<SegEntry *>(lump.payload());
    for (int i = 0; i < numSegs; i++) {
        mSegs.push_back({mVertices.at(entry->vertexStartId), mVertices.at(entry->vertexEndId)});
        entry++;
    }
}

void Map::parseBlockMap(Lump &lump) {
    assert(!strncmp(lump.name().c_str(), "BLOCKMAP", 8));
}

void Map::parseRejects(Lump &lump) {
    assert(!strncmp(lump.name().c_str(), "REJECT", 6));
}

void Map::parseSectors(Lump &lump) {
    assert(!strncmp(lump.name().c_str(), "SECTORS", 7));
}

void Map::parseThings(Lump &lump) {
    assert(!strncmp(lump.name().c_str(), "THINGS", 6));
}

void Map::setId(int i) {
    mId = i;
}

void Map::setEpisode(int i) {
    mEpisode = i;
}

void Map::buildImplicitSubSectors() {
    // Start with a subsector covering the level AABB.
    Polygon wholeMap;
    wholeMap.addPoint(mBoundingBox.min);
    wholeMap.addPoint({mBoundingBox.min.x, mBoundingBox.max.y});
    wholeMap.addPoint(mBoundingBox.max);
    wholeMap.addPoint({mBoundingBox.max.x, mBoundingBox.min.y});

    // Traverse BSP. The root of the BSP is the last node.
    visitNodeR(mNodes.at(mNodes.size() - 1), wholeMap);
}

void Map::splitPolygon(Plan &plan, Polygon &polygon, Polygon &left, Polygon &right) {
    left.clear();
    right.clear();

    if (polygon.size() == 0)
        return;

    for (int i = 0; i < polygon.size(); i++) {
        Point &lStart = polygon.at(i);
        Point &lEnd = polygon.at(i + 1);
        Plan::Side sideStart = plan.side(lStart);
        Plan::Side sideEnd = plan.side(lEnd);
        if (sideEnd == Plan::Side::OnPlan) {
//            Point intersection = plan.findIntersection(lStart, lEnd);
            left.addPoint(lEnd);
            right.addPoint(lEnd);
            continue;
        }

        if (sideStart == sideEnd) {
            if (sideStart == Plan::Side::Left) {
                left.addPoint(lEnd);
            } else {
                right.addPoint(lEnd);
            }
            continue;
        }

        Point intersection = plan.findIntersection(lStart, lEnd);

        if (sideEnd == Plan::Side::Left) {
            right.addPoint(intersection);
            left.addPoint(intersection);
            left.addPoint(lEnd);
        } else {
            left.addPoint(intersection);
            right.addPoint(intersection);
            right.addPoint(lEnd);
        }
    }

}

void Map::clipPolygon(Polygon &clipped, Polygon &polygon, Map::Segs &segs) {
//    clipped = polygon;
//    return;
    for (Seg seg : segs) {
        Plan p(seg.start, seg.end);
        Polygon leftPoly;
        Polygon rightPoly;
        splitPolygon(p, polygon, leftPoly, rightPoly);
        polygon = leftPoly;
    }
    clipped = polygon;
}

void Map::visitNodeR(Node &node, Polygon &polygon) {

    // Clipped inot nothingness
    if (polygon.size() == 0)
        return;

    // Make a plan with the splitting Seg
    Plan plan(node.start, node.end);

    Polygon leftSubPolygon;
    Polygon rightSubPolygon;
    splitPolygon(plan, polygon, leftSubPolygon, rightSubPolygon);

//    mImplicitSubectors.clear();
//    mImplicitSubectors.push_back(leftSubPolygon);
//    mImplicitSubectors.push_back(rightSubPolygon);
//    SVGRenderer renderer;
//    renderer.renderNodesAndSubSectors(*this);

    // Recurse if left is node or end if left is SubSector
    if (node.left & 0x8000) {
        Polygon clipped;
        uint16_t subSectorId = node.left & (~0x8000);
        clipPolygon(clipped, leftSubPolygon, subSectors().at(subSectorId)->segs());
        mImplicitSubectors.push_back(clipped);
    } else {
        uint16_t nodeId = node.left;
        visitNodeR(mNodes.at(nodeId), leftSubPolygon);
    }

//     Recurse if right is node or end if right is SubSector
    if (node.right & 0x8000) {
        Polygon clipped;
        uint16_t subSectorId = node.right & (~0x8000);
        clipPolygon(clipped, rightSubPolygon, subSectors().at(subSectorId)->segs());
        mImplicitSubectors.push_back(clipped);
    } else {
        uint16_t nodeId = node.right;
        visitNodeR(mNodes.at(nodeId), rightSubPolygon);
    }
}


