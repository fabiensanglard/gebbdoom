//
// Created by fabien sanglard on 2017-11-23.
//

#ifndef WADEXPLORER_SVGRENDERER_H
#define WADEXPLORER_SVGRENDERER_H


#include "Map.h"
#include "MapReader.h"

class SVGRenderer {

public:
    void render(MapReader::Maps maps);

private:
    void renderMap(Map& map);

    void renderNodes(Map &map);

    void renderLineDefs(Map &map);

    void renderSegs(Map &map);

    void generateSVGHeader(std::ofstream& os, Map& map);

    void renderSubSectors(Map &map);

    void renderBluePrint(Map &map);

    void renderSpecialLines(Map& map);

public:
    void renderNodesAndSubSectors(Map &map);

    void renderBlockmap(Map &map);

    void renderBspTree(Map &map);
};


#endif //WADEXPLORER_SVGRENDERER_H
