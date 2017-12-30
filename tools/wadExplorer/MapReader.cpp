//
// Created by fabien sanglard on 2017-11-23.
//

#include <iostream>
#include "MapReader.h"

void MapReader::parse(WadArchive::Lumps &lumps) {
        mCursor = 0;
        while (hasMore(lumps)) {
            Lump &lump = getNextLump(lumps);

            bool isMap = false;
            int episode = 0;
            int id=0;
            if (lump.name()[0] == 'E' && lump.name()[2] == 'M') {
                isMap = true;
                episode = lump.name()[1] - 48;
                id = lump.name()[3] - 48;
            }
            else if(!strncmp(lump.name().c_str(), "MAP", 3)) {
                isMap = true;
                episode = lump.name()[3] - 48;
                id = lump.name()[4] - 48;
            }

            if (isMap) {
                parseMap(episode, id, lumps);
            }




        }
}

void MapReader::parseMap(int episode, int id, WadArchive::Lumps& lumps) {
    Map *map = new Map();
    map->setId(id);
    map->setEpisode(episode);
    mMaps.push_back(map);

    // I want to parse the vertices before anything else, store lumps
    // and parse then in the order i need just after.
    Lump& thingsLump = getNextLump(lumps);
    Lump& linesDefsLump = getNextLump(lumps);
    Lump& sideDefsLump = getNextLump(lumps);
    Lump& verticesLump = getNextLump(lumps);
    Lump& segsLump = getNextLump(lumps);
    Lump& subSectorsLump = getNextLump(lumps);
    Lump& nodesLump = getNextLump(lumps);
    Lump& sectorsLump = getNextLump(lumps);
    Lump& rejectsLump = getNextLump(lumps);
    Lump& blockmapsLump = getNextLump(lumps);

    map->parseVertices(verticesLump); // Vertices must be parsed first.
    map->parseThings(thingsLump);
    map->parseLineDefs(linesDefsLump);
    map->parseSideDefs(sideDefsLump);
    map->parseSegs(segsLump);
    map->parseSubSectors(subSectorsLump); // SubSector must be parsed before Nodes.
    map->parseNodes(nodesLump);
    map->parseSectors(sectorsLump);
    map->parseRejects(rejectsLump);
    map->parseBlockMap(blockmapsLump);

    map->buildImplicitSubSectors();
}


