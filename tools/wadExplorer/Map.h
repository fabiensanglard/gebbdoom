//
// Created by fabien sanglard on 2017-11-23.
//

#ifndef WADEXPLORER_MAP_H
#define WADEXPLORER_MAP_H

#include <vector>
#include <cmath>
#include "Lump.h"

struct Point {
    int16_t x;
    int16_t y;

    Point() {}
    Point(int16_t inX, int16_t inY) : x(inX), y(inY) {}

    void rotate(float degree) {
        float radianAngle = degree * 2 * 3.1415 / 360;
        int16_t newX = cos(radianAngle) * x - sin(radianAngle) * y;
        int16_t newY = sin(radianAngle) * x + cos(radianAngle) * y;
        x = newX;
        y = newY;
    }

    Point substract(Point &point) {
        return Point(x - point.x, y - point.y);
    }

    Point multiply(double d) {
        return Point(x * d, y * d);
    }
};

struct Node {
    Point start;
    Point end;
    
    int16_t left;
    int16_t right;
};

#define LINE_FLAG_SECRET (1 << 5)
#define LINE_FLAG_BLOCK_SOUND (1 << 6)

struct LineDef {
    Point start;
    Point end;
    int16_t leftSideId;
    int16_t rightSideId;
    int16_t flags;

    bool isOneSided() {
        return leftSideId == -1;
    }

    bool hasFlag(size_t flag) {
        return (flags & flag) == flag;
    }

    bool isSecret() {
        return hasFlag(LINE_FLAG_SECRET);
    }

    bool isBlockSound() {
        return hasFlag(LINE_FLAG_BLOCK_SOUND);
    }

};


struct Seg {
    Point start;
    Point end;

    bool isEmpty() {
        return start.x == end.x && start.y == end.y;
    }
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
           return Right;
       } else if (length < 0) {
           return Left;
       }
       return OnPlan;
    }

    // Source: https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line
    // Used when we know there is an intersection.
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

    // https://gamedev.stackexchange.com/questions/44720/line-intersection-from-parametric-equation
    bool findSegIntersection(Point& start, Point& end, Point& intersection) {
        // Plan         = a+t.b
        // Polygon Edge = c+u.d

        float cx = start.x;
        float cy = start.y;
        float dx = end.x - start.x;
        float dy = end.y - start.y;

        float ax = mStart.x;
        float ay = mStart.y;
        float bx = mDelta.x;
        float by = mDelta.y;

        float demoninator = (dx * by-dy * bx);

        // Parallel, so no intersection
        if (demoninator < 0.001 && demoninator > -0.001)
            return false;

        float u=(bx * (cy-ay) + by * (ax-cx))/demoninator;

        // No intersection
        if (u <0 || u > 1)
            return  false;

        intersection = {start.x + u * (end.x - start.x), start.y + u * (end.y - start.y)};
        return true;
    }

    Point& start() {
        return mStart;
    }

    Point& end() {
        return mEnd;
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
    std::vector<std::vector<uint16_t>> blocksLines;
    std::vector<std::vector<Seg>> segs;
};

struct Thing {
    Point position;
    short type;
    // 0 is east, 90 is north, 180 is west, 270 is south. This value is only used for monsters, player
    // starts, deathmatch starts, and teleporter landing spots. Other
    // things look the same from all directions. Values are rounded to
    // the nearest 45 degree angle, so if the value is 80, it will actually face 90 - north.
    short angle;
    short flags;
};

struct Splitter {
    Point start;
    Point end;
    int depth;
    Plan plan;
    Polygon polygon;
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
        return mImplicitSubsectors;
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

    using Splitters = std::vector<Splitter>;
    Splitters& splitters() {
        return mSplitters;
    }

    size_t bspMaxDepth() {
        return mBspMaxDepth;
    }
private:
    Things mThings;
    AABB mBoundingBox;
    Vertices mVertices;
    Lines mLinesDegs;
    Segs mSegs;
    Nodes mNodes;
    SubSectors mSubSectors;
    Polygons mImplicitSubsectors;
    int mEpisode;
    int mId;

    BlockMap mBlockMap;

    void visitNodeR(Node& node, Polygon& polygon, size_t depth = 0);

    void splitPolygon(Plan& plan, Polygon& sector, Polygon& left, Polygon& right);

    void clipPolygon(Polygon &tclipped, Polygon& polygon, Segs &plans);

    void spliLine(Point point, Point anEnd, Plan &plan, Seg &left, Seg &right);

    Splitters mSplitters;

    void buildSplitter(Plan &plan, Polygon &polygon, size_t depth);

    size_t mBspMaxDepth = 0;
};


#endif //WADEXPLORER_MAP_H
