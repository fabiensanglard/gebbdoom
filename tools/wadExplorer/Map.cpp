//
// Created by fabien sanglard on 2017-11-23.
//

#include <assert.h>
#include <iostream>
#include <cfloat>
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

struct BlockmapHeader {
    short origin_x;
    short origin_y;
    short numColumns;
    short numRows;
};

struct ThingEntry {
    short x;
    short y;
    short angle;
    short type;
    short flags;
};

struct SectorEntry {
    int16_t floor_height;
    int16_t ceiling_height;
    char floorTexture[8];
    char ceilingTexture[8];
    short lightLevel;
    short type; // For lightning, damage,
    short tag; // Correspond to linedefID. Target of tripwire.
};

void Map::parseNodes(Lump &lump) {
    assert(!strncmp(lump.name().c_str(), "NODES", 5));
    size_t numNodes = lump.size() / 28;
    NodeEntry *entry = reinterpret_cast<NodeEntry *>(lump.payload());
    for (int i = 0; i < numNodes; i++) {
        Node node = {{entry->xstart, entry->ystart},
                     {entry->xstart + entry->dx, entry->ystart + entry->dy},
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
        LineDef lineDef = {start, end, entry->leftSideDef, entry->rightSideDef, entry->flags};
        mLinesDegs.push_back(lineDef);
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
    BlockmapHeader *header = (BlockmapHeader *) lump.payload();
    mBlockMap.origin.x = header->origin_x;
    mBlockMap.origin.y = header->origin_y;
    mBlockMap.numColumns = header->numColumns;
    mBlockMap.numRows = header->numRows;

    size_t numBlocks = header->numColumns * header->numRows;
    uint16_t *blockOffsetRover = (uint16_t *) (lump.payload() + 8);
    for (int i = 0; i < numBlocks; i++) {
        uint16_t offset = *blockOffsetRover;
        std::vector<uint16_t> lines;
        // Read list of lines belonging to this block
        uint16_t *blockReader = ((uint16_t *) lump.payload()) + offset;
        assert(*blockReader == 0x0000);
        blockReader++;
        while (*blockReader != 0xFFFF) {
            lines.push_back(*blockReader);
            blockReader++;
        }
        assert(*blockReader == 0xFFFF);
        mBlockMap.blocksLines.push_back(std::move(lines));
        blockOffsetRover++;
    }

    mBlockMap.segs.assign(mBlockMap.numRows * mBlockMap.numColumns, std::vector<Seg>());
    for (int16_t row = 0; row < mBlockMap.numRows; row++) {
        for (int16_t column = 0; column < mBlockMap.numColumns; column++) {
            size_t blockIndex = row * mBlockMap.numColumns + column;
            std::vector<uint16_t> &blocks = mBlockMap.blocksLines.at(blockIndex);

            // Create clipping lines
            Point clipStart = {mBlockMap.origin.x + column * 128, mBlockMap.origin.y + row * 128};
            Point clipEnd = {clipStart.x + 128, clipStart.y + 128};
            Plan vleft( {clipStart.x, clipStart.y}, {clipStart.x, clipEnd.y});
            Plan vright({  clipEnd.x, clipStart.y}, {  clipEnd.x, clipEnd.y});
            Plan hbotton({clipStart.x, clipStart.y}, {clipEnd.x, clipStart.y});
            Plan hltop(  {clipStart.x, clipEnd.y}, {clipEnd.x, clipEnd.y});

            for (auto index : blocks) {

                // Clip lines int segments with all four clipping lines.

                LineDef line = mLinesDegs.at(index);
                Seg splittedLineLeft;
                Seg splittedLineRight;
                spliLine(line.start, line.end, vleft, splittedLineLeft, splittedLineRight);
                spliLine(splittedLineRight.start, splittedLineRight.end, vright, splittedLineLeft, splittedLineRight);
                spliLine(splittedLineLeft.start, splittedLineLeft.end, hbotton, splittedLineLeft, splittedLineRight);
                spliLine(splittedLineLeft.start, splittedLineLeft.end, hltop, splittedLineLeft, splittedLineRight);
                if (!splittedLineRight.isEmpty())
                    mBlockMap.segs[blockIndex].push_back(splittedLineRight);
            }
        }
    }

}

void Map::parseRejects(Lump &lump) {
    assert(!strncmp(lump.name().c_str(), "REJECT", 6));
}

void Map::parseSectors(Lump &lump) {
    assert(!strncmp(lump.name().c_str(), "SECTORS", 7));
}

void Map::parseThings(Lump &lump) {
    assert(!strncmp(lump.name().c_str(), "THINGS", 6));
    size_t numThings = lump.size() / 10;
    ThingEntry *entry = reinterpret_cast<ThingEntry *>(lump.payload());
    for (int i = 0; i < numThings; i++) {
        Thing thing;
        thing.position.x = entry->x;
        thing.position.y = entry->y;
        thing.angle = entry->angle;
        thing.type = entry->type;
        thing.flags = entry->flags;
        entry++;
        mThings.push_back(thing);
    }
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
            left.addPoint(lEnd);
            right.addPoint(lEnd);
            continue;
        }

        if (sideStart == sideEnd) {
            if (sideStart == Plan::Side::Left) {
                right.addPoint(lEnd);
            } else {
                left.addPoint(lEnd);
            }
            continue;
        }

        Point intersection = plan.findIntersection(lStart, lEnd);

        if (sideEnd == Plan::Side::Left) {
            left.addPoint(intersection);
            right.addPoint(intersection);
            right.addPoint(lEnd);
        } else {
            right.addPoint(intersection);
            left.addPoint(intersection);
            left.addPoint(lEnd);
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

void Map::visitNodeR(Node &node, Polygon &polygon, size_t depth) {

    if (depth > mBspMaxDepth)
        mBspMaxDepth = depth;

    // Clipped inot nothingness
    if (polygon.size() == 0)
        return;

    // Make a plan with the splitting Seg
    Plan plan(node.start, node.end);

    // Build the long splitter.
    buildSplitter(plan, polygon, depth);

    Polygon leftSubPolygon;
    Polygon rightSubPolygon;
    splitPolygon(plan, polygon, leftSubPolygon, rightSubPolygon);

    // Recurse if left is node or end if left is SubSector
    if (node.left & 0x8000) {
        Polygon clipped;
        uint16_t subSectorId = node.left & (~0x8000);
        clipPolygon(clipped, leftSubPolygon, subSectors().at(subSectorId)->segs());
        mImplicitSubsectors.push_back(clipped);
    } else {
        uint16_t nodeId = node.left;
        visitNodeR(mNodes.at(nodeId), leftSubPolygon, depth + 1);
    }

//     Recurse if right is node or end if right is SubSector
    if (node.right & 0x8000) {
        Polygon clipped;
        uint16_t subSectorId = node.right & (~0x8000);
        clipPolygon(clipped, rightSubPolygon, subSectors().at(subSectorId)->segs());
        mImplicitSubsectors.push_back(clipped);
    } else {
        uint16_t nodeId = node.right;
        visitNodeR(mNodes.at(nodeId), rightSubPolygon, depth + 1);
    }
}

void Map::spliLine(Point start, Point end, Plan &plan, Seg &left, Seg &right) {
    left =  {{0,0}, {0,0}};
    right = {{0,0}, {0,0}};
    Plan::Side sideStart = plan.side(start);
    Plan::Side sideEnd = plan.side(end);
    if (sideEnd == Plan::Side::OnPlan) {
        left = {start, end};
        right = {start, end};
        return;
    }

    if (sideStart == sideEnd) {
        if (sideStart == Plan::Side::Left) {
            left = {start, end};
        } else {
            right = {start, end};
        }
        return;
    }

    Point intersection = plan.findIntersection(start, end);

    if (sideEnd == Plan::Side::Left) {
        left = {intersection, end};
        right = {start, intersection};
    } else {
        left = {start, intersection};
        right = {intersection, end};

    }
}

void Map::buildSplitter(Plan &plan, Polygon &polygon, size_t depth) {
    std::vector<Point> intersections;
    // For each line in the polygon, calculate intersection.
    for (int i = 0; i < polygon.size(); i++) {
        Point &lStart = polygon.at(i);
        Point &lEnd = polygon.at(i + 1);
        Point intersection;
        bool intersect = plan.findSegIntersection(lStart, lEnd, intersection);

        if (!intersect)
            continue;

        intersections.push_back(intersection);
    }
    if (intersections.size() != 2)
        return;
    mSplitters.push_back({intersections[0], intersections[1], depth, plan, polygon});
}

