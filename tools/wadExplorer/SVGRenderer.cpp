//
// Created by fabien sanglard on 2017-11-23.
//

#include <fstream>
#include <sstream>
#include "SVGRenderer.h"
#include "Map.h"
#include "MapReader.h"
#include "Things.h"

std::string svgStart2 = R"foo(
 xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
   <defs>
    <marker id="arrow" markerWidth="10" markerHeight="10" refX="0" refY="2" orient="auto" markerUnits="strokeWidth">
      <path d="M0,0 L0,4 L5,3 z" fill="#f00" />
    </marker>
    <linearGradient id="black-linearh">
      <stop offset="0%"  stop-color="rgb(0,0,0)" stop-opacity="90"/>
      <stop offset="90%"  stop-color="rgb(0,0,0)" stop-opacity="50"/>
      <stop offset="100%" stop-color="rgb(0,0,0)" stop-opacity="0"/>
    </linearGradient>

    <linearGradient id="black-linearv" x1="0" x2="0" y1="0" y2="1">
      <stop offset="0%"  stop-color="rgb(0,0,0)" stop-opacity="90"/>
      <stop offset="50%"  stop-color="rgb(0,0,0)" stop-opacity="90"/>
      <stop offset="100%" stop-color="rgb(0,0,0)" stop-opacity="0"/>
    </linearGradient>

    <linearGradient id="red-linearh">
      <stop offset="0%"  stop-color="rgb(206, 17, 74)" stop-opacity="100"/>
      <stop offset="90%"  stop-color="rgb(206, 17, 74)" stop-opacity="90"/>
      <stop offset="100%" stop-color="rgb(206,17,74)" stop-opacity="0"/>
    </linearGradient>

    <linearGradient id="red-linearv" x1="0" x2="0" y1="0" y2="1">
      <stop offset="0%"  stop-color="rgb(206, 17, 74)" stop-opacity="100"/>
      <stop offset="90%"  stop-color="rgb(206, 17, 74)" stop-opacity="90"/>
      <stop offset="100%" stop-color="rgb(206,17,74)" stop-opacity="0"/>
    </linearGradient>
  </defs>
)foo";

//<stop offset="100%" stop-color="rgb(188,123,143)" stop-opacity="30"/>
std::string epilogue = R"(
</svg>
)";

void SVGRenderer::generateSVGHeader(std::ofstream &os, Map &map) {
    constexpr int kpadding = 130;
    constexpr int kwidth = 900;

    AABB &aabb = map.boundingBox();

    // Add some padding
    float ratio = aabb.height / (float) aabb.width;
    os
            << "<svg width=\""
            << kwidth
            << "\" height=\""
            << (int) (kwidth * ratio)
            << "\" preserveAspectRatio=\"none\""
            << " viewBox=\""
            << (int(aabb.min.x)) - kpadding
            << " "
            << int(aabb.min.y * -1) - aabb.height - kpadding
            << " "
            << aabb.width + kpadding * 2
            << " "
            << aabb.height + kpadding * 2
            << "\""
            << svgStart2

            // Flip coordinate system to have origin in lower left instead of upper left.
            //            << "<g transform=\"translate(0,"
            //            << height
            //            << ")\"/>"
            //            << "<g transform=\"scale(1,-1)\"/>"
            << std::endl;
}

void SVGRenderer::render(MapReader::Maps maps) {
    for (Map *map : maps) {
        renderMap(*map);
//        break;
    }
}

void SVGRenderer::renderMap(Map &map) {
    renderNodes(map);
    renderSpecialLines(map);
    renderLineDefs(map);
    renderSegs(map);
    renderSubSectors(map);
    renderNodesAndSubSectors(map);
    renderBlockmap(map);
    renderBluePrint(map);
    renderBspTree(map);
}

void SVGRenderer::renderSpecialLines(Map &map) {
    std::ostringstream str;
    str << "E" << map.episode() << "M" << map.id() << "_special_lines.svg";
    std::string filename = str.str();
    std::ofstream outputFile(filename);
    generateSVGHeader(outputFile, map);

    for (LineDef line : map.lines()) {
        outputFile << "<line x1=\"" << line.start.x
                   << "\" y1=\"" << -line.start.y
                   << "\" x2=\"" << line.end.x
                   << "\" y2=\"" << -line.end.y
                   << "\" stroke-width=\"";
        if (line.isOneSided()) {
            outputFile << 7;
            outputFile << "\" stroke=\"black\" />";
        } else {
            if (line.isBlockSound()) {
                outputFile << 17;
                outputFile << "\" stroke=\"red\" />";
            } else {
            outputFile << 7;
            outputFile << "\" stroke=\"gray\" />";
            }
        }
        outputFile << std::endl;

        outputFile << "    <circle cx=\""
                   << line.start.x
                   << "\" cy=\""
                   << -line.start.y
                   << "\" r=\""
                   << 10
                   << "\"/>";

        outputFile << "    <circle cx=\""
                   << line.end.x
                   << "\" cy=\""
                   << -line.end.y
                   << "\" r=\""
                   << 10
                   << "\"/>"
                   << std::endl;
    }

    // Draw things now.
    for (Thing thing : map.getThings()) {
        struct color {
            int r;
            int g;
            int b;} color = {0,0,0};
        if (isMonster(thing.type)) {
            color = {165, 28, 28};
        }
        if (isPlayer(thing.type)) {
            color = {77, 193, 60};
        }
        uint16_t radius = 15;
        if (thingsDefs.find(thing.type) != thingsDefs.end()) {
            ThingDef &def = thingsDefs.at(thing.type);
            radius = def.radius;
        }
        // Show level 1 only
//            if ((thing.flags & THING_LEVEL1_2) != THING_LEVEL1_2)
//                continue;

        // Show level 1-2-3
//            if ((thing.flags & THING_LEVEL3) != THING_LEVEL3)
//                continue;
//
        // Show all
//            if ((thing.flags & THING_LEVEL4_5) != THING_LEVEL4_5)
//                continue;

//            int level = thing.flags & 12;
//            if (level  > 2)
//                continue;

        outputFile << "    <circle cx=\""
                   << thing.position.x
                   << "\" cy=\""
                   << -thing.position.y
                   << "\" r=\""
                   << radius
                   << "\" stroke=\"rgb(0,0,0)\" stroke-width=\"5\" fill=\"rgb("
                   << color.r
                   << ","
                   << color.g
                   << ","
                   << color.b
                   << ")"

                   << "\"/>"
                   << std::endl;


    }

    outputFile << epilogue << std::endl;
    outputFile.close();
}

void SVGRenderer::renderBluePrint(Map &map) {
    std::ostringstream str;
    str << "E" << map.episode() << "M" << map.id() << "_blueprint.svg";
    std::string filename = str.str();
    std::ofstream outputFile(filename);
    generateSVGHeader(outputFile, map);

    for (Thing thing : map.getThings()) {
        outputFile << "    <circle cx=\""
                   << thing.position.x
                   << "\" cy=\""
                   << -thing.position.y
                   << "\" r=\""
                   << 18
                   << "\" fill=\"rgb(170,170,170)"
                   << "\"/>"
                   << std::endl;
    }

    for (LineDef line : map.lines()) {
        Point delta = line.end.substract(line.start);
        double random = (rand() % 1000) / 4999.9f;
        delta = delta.multiply(random);
        outputFile << "<line x1=\"" << line.start.x - delta.x
                   << "\" y1=\"" << -(line.start.y - delta.y)
                   << "\" x2=\"" << float(line.end.x + delta.x + .01)
                   << "\" y2=\"" << -float(line.end.y + delta.y + .01)
                   << "\" stroke-width=\"";

        random = (rand() % 6) + 8;
        outputFile << random;

        if (line.isOneSided()) {
            if (delta.x > delta.y)
                outputFile << "\" stroke=\"url(#black-linearh)\"  stroke-linecap=\"round\"/>";
            else
                outputFile << "\" stroke=\"url(#black-linearv)\"  stroke-linecap=\"round\"/>";
        } else {
            if (delta.x > delta.y)
            outputFile << "\" stroke=\"url(#red-linearh)\" stroke-linecap=\"round\"/>";
            else
                outputFile << "\" stroke=\"url(#red-linearv)\" stroke-linecap=\"round\"/>";

        }
        outputFile << std::endl;
    }


    outputFile << epilogue << std::endl;
    outputFile.close();
}

void SVGRenderer::renderNodes(Map &map) {
    std::ostringstream str;
    str << "E" << map.episode() << "M" << map.id() << "_nodes.svg";
    std::string filename = str.str();
    std::ofstream outputFile(filename);
    generateSVGHeader(outputFile, map);

    for (Node node : map.nodes()) {
        outputFile << "<line x1=\"" << node.start.x
                   << "\" y1=\"" << -node.start.y
                   << "\" x2=\"" << node.end.x
                   << "\" y2=\"" << -node.end.y
                   << "\" stroke-width=\""
                   << 10
                   << "\" stroke=\"black\"/>"
                   << std::endl;
    }
    outputFile << epilogue << std::endl;
    outputFile.close();
}

void SVGRenderer::renderLineDefs(Map &map) {
    std::ostringstream str;
    str << "E" << map.episode() << "M" << map.id() << "_sides.svg";
    std::string filename = str.str();
    std::ofstream outputFile(filename);
    generateSVGHeader(outputFile, map);

    for (LineDef line : map.lines()) {
        outputFile << "<line x1=\"" << line.start.x
                   << "\" y1=\"" << -line.start.y
                   << "\" x2=\"" << line.end.x
                   << "\" y2=\"" << -line.end.y
                   << "\" stroke-width=\"";
        if (line.isOneSided()) {
            outputFile << 7;
            outputFile << "\" stroke=\"black\" />";
        } else {
//            if (line.isBlockSound()) {
//                outputFile << 17;
//                outputFile << "\" stroke=\"blue\" />";
//            } else {
                outputFile << 7;
                outputFile << "\" stroke=\"gray\" />";
//            }
        }
        outputFile << std::endl;

        outputFile << "    <circle cx=\""
                   << line.start.x
                   << "\" cy=\""
                   << -line.start.y
                   << "\" r=\""
                   << 10
                   << "\"/>";

        outputFile << "    <circle cx=\""
                   << line.end.x
                   << "\" cy=\""
                   << -line.end.y
                   << "\" r=\""
                   << 10
                   << "\"/>"
                   << std::endl;
    }

    // Draw things now.
    for (Thing thing : map.getThings()) {
        struct color {
                int r;
            int g;
            int b;} color = {0,0,0};
        if (isMonster(thing.type)) {
            color = {165, 28, 28};
        }
        if (isPlayer(thing.type)) {
            color = {77, 193, 60};
        }
        uint16_t radius = 15;
        if (thingsDefs.find(thing.type) != thingsDefs.end()) {
            ThingDef &def = thingsDefs.at(thing.type);
            radius = def.radius;
        }
            // Show level 1 only
//            if ((thing.flags & THING_LEVEL1_2) != THING_LEVEL1_2)
//                continue;

            // Show level 1-2-3
            if ((thing.flags & THING_LEVEL3) != THING_LEVEL3)
                continue;
//
            // Show all
//            if ((thing.flags & THING_LEVEL4_5) != THING_LEVEL4_5)
//                continue;

//            int level = thing.flags & 12;
//            if (level  > 2)
//                continue;



        // Draw vision field
//        continue;
        if (isMonster(thing.type)) {

            outputFile << "    <circle cx=\""
                       << thing.position.x
                       << "\" cy=\""
                       << -thing.position.y
                       << "\" r=\""
                       << radius
                       << "\" stroke=\"rgb(0,0,0)\" stroke-width=\"5\" fill=\"rgb("
                       << color.r
                       << ","
                       << color.g
                       << ","
                       << color.b
                       << ")"

                       << "\"/>"
                       << std::endl;

            Point leftTriangle = {730, 430};
            Point rightTriangle = {730, -430};
//            thing.angle = 70;
            leftTriangle.rotate(thing.angle);
            rightTriangle.rotate(thing.angle);
            outputFile << "<polygon points=\""
                       << thing.position.x
                       << ","
                       << -thing.position.y
                       << " "
                       << thing.position.x + leftTriangle.x
                       << ","
                       << -(thing.position.y + leftTriangle.y)
                       << " "
                       << thing.position.x + rightTriangle.x
                       << ","
                       << -(thing.position.y + rightTriangle.y)
                       << "\" style=\"fill:blue;stroke:dark;stroke-width:1\" opacity=\"0.25\" />"
                       << std::endl;
        }
    }
    outputFile << epilogue << std::endl;
    outputFile.close();
}

void SVGRenderer::renderSegs(Map &map) {
    std::ostringstream str;
    str << "E" << map.episode() << "M" << map.id() << "_segs.svg";
    std::string filename = str.str();
    std::ofstream outputFile(filename);

    generateSVGHeader(outputFile, map);

    for (Seg seg : map.segs()) {
        outputFile << "<line x1=\"" << seg.start.x
                   << "\" y1=\"" << -seg.start.y
                   << "\" x2=\"" << seg.end.x
                   << "\" y2=\"" << -seg.end.y
                   << "\" stroke-width=\""
                   << 7
                   << "\" stroke=\"black\" />" //marker-end=\"url(#arrow)\" />"
                   << std::endl;

        outputFile << "    <circle cx=\""
                   << seg.start.x
                   << "\" cy=\""
                   << -seg.start.y
                   << "\" r=\""
                   << 10
                   << "\"/>";

        outputFile << "    <circle cx=\""
                   << seg.end.x
                   << "\" cy=\""
                   << -seg.end.y
                   << "\" r=\""
                   << 10
                   << "\"/>"
                   << std::endl;
    }

    for(Splitter splitter: map.splitters()) {
        if (splitter.depth > 2)
            continue;

        float opacity = 1.0f;

        outputFile << "<line x1=\"" << splitter.start.x << "\" y1=\"" << -splitter.start.y
                   << "\"    x2=\"" << splitter.end.x << "\" y2=\"" << -splitter.end.y
                   << "\" stroke-width=\"" << 25 << "\" stroke=\"black\" "
                   << "stroke-opacity=\"" << opacity << "\" "
                   << "fill-opacity=\"" << opacity << "\""
                   << "/>" << std::endl;
    }

    outputFile << epilogue << std::endl;
    outputFile.close();
}

void SVGRenderer::renderSubSectors(Map &map) {
    std::ostringstream str;
    str << "E" << map.episode() << "M" << map.id() << "_subsectors.svg";
    std::string filename = str.str();
    std::ofstream outputFile(filename);

    generateSVGHeader(outputFile, map);

    for (SubSector *subSector: map.subSectors()) {
        if (map.episode() == 1 && map.id() == 1 && subSector->segs().size() > 1) {
            outputFile << " ";
        }
        int r, g, b;
        r = subSector->segs().at(0).start.x % 256;
        g = subSector->segs().at(0).start.y % 256;
        b = subSector->segs().at(0).end.x % 256;

        for (Seg seg : subSector->segs()) {
            outputFile << "<line x1=\"" << seg.start.x
                       << "\" y1=\"" << -seg.start.y
                       << "\" x2=\"" << seg.end.x
                       << "\" y2=\"" << -seg.end.y
                       << "\" stroke-width=\""
                       << 10
                       << "\" stroke=\"rgb("
                       << r
                       << ","
                       << g
                       << ","
                       << b
                       << ")\" marker-end=\"url(#arrow)\" />"
                       << std::endl;
        }
    }
    outputFile << epilogue << std::endl;
    outputFile.close();
}

void SVGRenderer::renderNodesAndSubSectors(Map &map) {
    std::ostringstream str;
    str << "E" << map.episode() << "M" << map.id() << "_fab.svg";
    std::string filename = str.str();
    std::ofstream outputFile(filename);

    generateSVGHeader(outputFile, map);


    for (Polygon polygon : map.implicitSubSectors()) {
        int r, g, b;
        int randomColor = rand() % 6 + 1;
        r = ((randomColor & 4) >> 2) * 255;
        g = ((randomColor & 2) >> 1) * 255;
        b = ((randomColor & 1) >> 0) * 255;

        float f = 0.15; // desaturate by f%
        float L = 0.3 * r + 0.6 * g + 0.1 * b;
        int new_r = r + f * (L - r);
        int new_g = g + f * (L - g);
        int new_b = b + f * (L - b);

        outputFile << "<polygon points=\"";
        for (Point point : polygon.points()) {
            outputFile << point.x;
            outputFile << ",";
            outputFile << -point.y;
            outputFile << " ";
        }
        outputFile << "\" fill=\"rgb(" << new_r << "," << new_g << "," << new_b << ")\""
                   << " stroke=\"rgb(" << 70 << "," << 70 << "," << 70 << ")"
                   << "\" stroke-width=\"10\" stroke-linecap=\"round\" />"
                   << std::endl;
    }

    outputFile << epilogue << std::endl;
    outputFile.close();
}

void SVGRenderer::renderBlockmap(Map &map) {
    std::ostringstream str;
    str << "E" << map.episode() << "M" << map.id() << "_blockmap.svg";
    std::string filename = str.str();
    std::ofstream outputFile(filename);

    generateSVGHeader(outputFile, map);
//    BlockMap &bm = map.getBlockmap();

    size_t blockMapId = 0;
    for (Segs segs: map.getBlockmap().segs) {
        size_t line = blockMapId / map.getBlockmap().numColumns;
        size_t column = blockMapId % map.getBlockmap().numColumns;

        int randomColor = (blockMapId + line % 6) % 6 + 1;
        int r = ((randomColor & 4) >> 2) * 255;
        int g = ((randomColor & 2) >> 1) * 255;
        int b = ((randomColor & 1) >> 0) * 255;
        blockMapId++;

        if (segs.size() == 0)
            continue;


        for (Seg seg : segs) {
            outputFile << "<line x1=\"" << seg.start.x
                       << "\" y1=\"" << -seg.start.y
                       << "\" x2=\"" << seg.end.x
                       << "\" y2=\"" << -seg.end.y
                       << "\" stroke-width=\""
                       << 13
                       //                       << "\" stroke=\"black\" />"
                       << "\" stroke=\"rgb(" << r << "," << g << "," << b << ")\"/>"
                       << std::endl;
        }

        // Draw block square.
        constexpr size_t gridStrokeWidth = 5;
        Point clipStart = {map.getBlockmap().origin.x + column * 128, map.getBlockmap().origin.y + line * 128};
        Point clipEnd = {clipStart.x + 128, clipStart.y + 128};
        outputFile << "<line x1=\"" << clipStart.x << "\" y1=\"" << -clipStart.y
                   << "\"    x2=\"" << clipStart.x << "\" y2=\"" << -clipEnd.y
                   << "\" stroke-width=\"" << gridStrokeWidth << "\" stroke=\"black\" />" << std::endl;

        outputFile << "<line x1=\"" << clipStart.x << "\" y1=\"" << -clipStart.y
                   << "\"    x2=\"" << clipEnd.x << "\"   y2=\"" << -clipStart.y
                   << "\" stroke-width=\"" << gridStrokeWidth << "\" stroke=\"black\" />" << std::endl;

        outputFile << "<line x1=\"" << clipEnd.x << "\" y1=\"" << -clipStart.y
                   << "\"    x2=\"" << clipEnd.x << "\" y2=\"" << -clipEnd.y
                   << "\" stroke-width=\"" << gridStrokeWidth << "\" stroke=\"black\"/>" << std::endl;

        outputFile << "<line x1=\"" << clipStart.x << "\" y1=\"" << -clipEnd.y
                   << "\"    x2=\"" << clipEnd.x << "\"   y2=\"" << -clipEnd.y
                   << "\" stroke-width=\"" << gridStrokeWidth << "\" stroke=\"black\"/>" << std::endl;

    }

    outputFile << epilogue << std::endl;
    outputFile.close();
}

void SVGRenderer::renderBspTree(Map &map) {
    std::ostringstream str;
    str << "E" << map.episode() << "M" << map.id() << "_bsptree.svg";
    std::string filename = str.str();
    std::ofstream outputFile(filename);

    generateSVGHeader(outputFile, map);

    for(Splitter splitter: map.splitters()) {
//        if (splitter.depth > 1)
//            continue;


//        int r, g, b;
//        int randomColor = rand() % 6 + 1;
//        r = ((randomColor & 4) >> 2) * 255;
//        g = ((randomColor & 2) >> 1) * 255;
//        b = ((randomColor & 1) >> 0) * 255;
//
//        float f = 0.15; // desaturate by f%
//        float L = 0.3 * r + 0.6 * g + 0.1 * b;
//        int new_r = r + f * (L - r);
//        int new_g = g + f * (L - g);
//        int new_b = b + f * (L - b);
//        outputFile << "<polygon points=\"";
//        for (Point point : splitter.polygon.points()) {
//            outputFile << point.x;
//            outputFile << ",";
//            outputFile << -point.y;
//            outputFile << " ";
//        }
//        outputFile << "\" fill=\"rgb(" << new_r << "," << new_g << "," << new_b << ")\""
//                   << " stroke=\"rgb(" << 70 << "," << 70 << "," << 70 << ")"
//                   << "\" stroke-width=\"10\" stroke-linecap=\"round\" />"
//                   << std::endl;

        float opacity = (map.bspMaxDepth() - splitter.depth) / float(map.bspMaxDepth()) ;

        outputFile << "<line x1=\"" << splitter.start.x << "\" y1=\"" << -splitter.start.y
                   << "\"    x2=\"" << splitter.end.x << "\" y2=\"" << -splitter.end.y
                   << "\" stroke-width=\"" << 15   << "\" stroke=\"black\" "
                   << "stroke-opacity=\""   <<opacity <<"\" "
                   << "fill-opacity=\""   <<opacity <<"\""
                << "/>" << std::endl;

//        outputFile << "<line x1=\"" << splitter.plan.start().x << "\" y1=\"" << -splitter.plan.start().y
//                   << "\"    x2=\"" << splitter.plan.end().x << "\" y2=\"" << -splitter.plan.end().y
//                   << "\" stroke-width=\"" << (map.bspMaxDepth() - splitter.depth) << "\" stroke=\"red\" />" << std::endl;

    }

    outputFile << epilogue << std::endl;
    outputFile.close();
}
