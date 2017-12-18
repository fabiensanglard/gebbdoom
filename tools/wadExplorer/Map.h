//
// Created by fabien sanglard on 2017-11-23.
//

#ifndef WADEXPLORER_MAP_H
#define WADEXPLORER_MAP_H

#include <vector>
#include "Lump.h"

struct Point {
    int16_t x;
    int16_t y;
};

struct Node {
    Point start;
    Point end;
    
    int16_t left;
    int16_t right;
};

struct LineDef {
    Point start;
    Point end;
};


struct Seg {
    Point start;
    Point end;
};

using Segs = std::vector<Seg>;
class SubSector {
public:
    SubSector() = default;

    void addSeg(Seg& seg) {
        mSegs.push_back(seg);
    }

    void addSeg(Seg&& seg) {
        mSegs.push_back(seg);
    }

    Segs& segs() {
        return mSegs;
    }

private:
    Segs mSegs;
};

struct AABB {
    AABB(){}
    AABB(Point& inMin, Point& inMax) {
        min = inMin;
        max = inMax;
        width = max.x - min.x;
        height = max.y - min.y;
    }
    Point min;
    Point max;
    ushort width;
    ushort height;
};

class Plan {
public:
    Plan(Point& start, Point& end) {
        mStart = start;
        mEnd = end;
        mDelta.x = end.x - start.x;
        mDelta.y = end.y - start.y;
    }
    Plan(Point&& start, Point&& end) {
        mStart = start;
        mEnd = end;
        mDelta.x = end.x - start.x;
        mDelta.y = end.y - start.y;
    }
    // Ux*Vy-Uy*Vx
    Plan(Seg &seg) : Plan(seg.start, seg.end) {

    }

    enum Side{Left, Right, OnPlan};

    Side side(Point& point) {
       Point pointDelta = {mStart.x - point.x, mStart.y - point.y};
       float length = mDelta.x * pointDelta.y - mDelta.y * pointDelta.x;
       if (length > 0) {
           return Left;
       } else if (length < 0) {
           return Right;
       }
       return OnPlan;
    }

    // Source: https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line
    Point findIntersection(Point& start, Point& end) {
        // U
        float x1 = mStart.x;
        float y1 = mStart.y;
        float x2 = mEnd.x;
        float y2 = mEnd.y;

        // V
        float x3 = start.x;
        float y3 = start.y;
        float x4 = end.x;
        float y4 = end.y;

        float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

        float xNominator =  (x1*y2 - y1*x2) * (x3 - x4) - (x1 - x2) * (x3*y4 - y3*x4);
        float yNominator =  (x1*y2 - y1*x2) * (y3 - y4) - (y1 - y2) * (x3*y4 - y3*x4);
        Point intersection = {xNominator / denominator,
                              yNominator / denominator};
        return intersection;
    }


private:
    Point mStart;
    Point mEnd;
    Point mDelta;
};

class Polygon {
public:
    void addPoint(Point& point){
        mPoints.push_back(point);
    }
    void addPoint(Point&& point){
        mPoints.push_back(point);
    }
    inline size_t size() {
        return mPoints.size();
    }

    inline Point& at(size_t i) {
        return mPoints.at(i % size());
    }
    void clear() {
        mPoints.clear();
    }
    std::vector<Point>& points() {
        return mPoints;
    }
private:
    std::vector<Point> mPoints;
};

struct BlockMap {
    Point origin;
    short numRows;
    short numColumns;
    std::vector<std::vector<uint16_t>> blocks;
    std::vector<std::vector<Seg>> segs;
};

struct Thing {
    Point position;
    short type;
    short angle;
    short flags;
};

class Map {
public:
    void parseNodes(Lump& lump);
    void parseLineDefs(Lump &lump);
    void parseSideDefs(Lump &lump);
    void parseVertices(Lump &lump);
    void parseSegs(Lump &lump);
    void parseSubSectors(Lump &lump);
    void parseSectors(Lump &lump);
    void parseRejects(Lump &lump);
    void parseBlockMap(Lump &lump);
    void parseThings(Lump &lump);

    using Nodes = std::vector<Node>;
    Nodes& nodes() {
        return mNodes;
    }

    using Lines = std::vector<LineDef>;
    Lines& lines() {
        return mLinesDegs;
    }

    using Segs = std::vector<Seg>;
    Segs& segs() {
            return mSegs;
    };

    int episode() {
        return mEpisode;
    }

    int id() {
        return mId;
    }

    void setId(int i);

    void setEpisode(int i);

    using Vertices = std::vector<Point>;
    Vertices& vertices() {
        return mVertices;
    }

    using SubSectors = std::vector<SubSector*>;
    SubSectors& subSectors() {
        return mSubSectors;
    }

    using Polygons = std::vector<Polygon>;
    Polygons& implicitSubSectors() {
        return mImplicitSubectors;
    }

    AABB& boundingBox() {
        return mBoundingBox;
    }

    using Things = std::vector<Thing>;
    Things& getThings() {
        return mThings;
    };

    BlockMap& getBlockmap() {
        return mBlockMap;
    }

    void buildImplicitSubSectors();
private:
    Things mThings;
    AABB mBoundingBox;
    Vertices mVertices;
    Lines mLinesDegs;
    Segs mSegs;
    Nodes mNodes;
    SubSectors mSubSectors;
    Polygons mImplicitSubectors;
    int mEpisode;
    int mId;

    BlockMap mBlockMap;

    void visitNodeR(Node& node, Polygon& polygon);

    void splitPolygon(Plan& plan, Polygon& sector, Polygon& left, Polygon& right);

    void clipPolygon(Polygon &tclipped, Polygon& polygon, Segs &plans);

    void spliLine(Point &point, Point &anEnd, Plan &plan, Seg &left, Seg &right);
};


#endif //WADEXPLORER_MAP_H
