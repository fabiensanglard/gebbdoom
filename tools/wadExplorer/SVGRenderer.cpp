//
// Created by fabien sanglard on 2017-11-23.
//

#include <fstream>
#include <sstream>
#include "SVGRenderer.h"
#include "Map.h"
#include "MapReader.h"

std::string svgStart2 = R"(
 xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
   <defs>
    <marker id="arrow" markerWidth="10" markerHeight="10" refX="0" refY="2" orient="auto" markerUnits="strokeWidth">
      <path d="M0,0 L0,4 L5,3 z" fill="#f00" />
    </marker>
  </defs>
)";

std::string epilogue = R"(
</svg>
)";

void SVGRenderer::generateSVGHeader(std::ofstream &os, Map &map) {
    constexpr int kpadding = 50;
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
    }
}

void SVGRenderer::renderMap(Map &map) {
    renderNodes(map);
    renderLineDefs(map);
    renderSegs(map);
    renderSubSectors(map);
    renderNodesAndSubSectors(map);
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

    for (LineDef side : map.lines()) {
        outputFile << "<line x1=\"" << side.start.x
                   << "\" y1=\"" << -side.start.y
                   << "\" x2=\"" << side.end.x
                   << "\" y2=\"" << -side.end.y
                   << "\" stroke-width=\""
                   << 7
                   << "\" stroke=\"black\" marker-end=\"url(#arrow)\" />"
                   << std::endl;

        outputFile << "    <circle cx=\""
                   << side.start.x
                   << "\" cy=\""
                   << -side.start.y
                   << "\" r=\""
                   << 10
                   << "\"/>";

        outputFile << "    <circle cx=\""
                   << side.end.x
                   << "\" cy=\""
                   << -side.end.y
                   << "\" r=\""
                   << 10
                   << "\"/>"
                   << std::endl;
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
                   << "\" stroke=\"black\" marker-end=\"url(#arrow)\" />"
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
        int randomColor = rand();
        r = randomColor >> 24 % 200;
        g = (randomColor >> 16) & 0xFF;
        b = (randomColor >> 8) & 0xFF;

        outputFile << "<polygon points=\"";
        for (Point point : polygon.points()) {
            outputFile << point.x;
            outputFile << ",";
            outputFile << -point.y;
            outputFile << " ";
        }
//        outputFile << "\" fill=\"transparent\" stroke=\"black\">"
        outputFile << "\" style=\"fill:"
                   << "rgb("
                   << r
                   << ","
                   << g
                   << ","
                   << b
                   << ")\"/>"
                   << std::endl;
    }

//    Node node = map.nodes().at(map.nodes().size() -1 ) ;
//        outputFile << "<line x1=\"" << node.start.x
//                   << "\" y1=\"" << -node.start.y
//                   << "\" x2=\"" << node.end.x
//                   << "\" y2=\"" << -node.end.y
//                   << "\" stroke-width=\""
//                   << 10
//                   << "\" marker-end=\"url(#arrow)\" stroke=\"black\"/>"
//                   << std::endl;
    for (Node node : map.nodes()) {
        break;
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
