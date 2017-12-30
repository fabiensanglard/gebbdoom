//
// Created by fabien sanglard on 2017-11-23.
//

#ifndef WADEXPLORER_MAPREADER_H
#define WADEXPLORER_MAPREADER_H


#include <vector>
#include "Map.h"
#include "WadArchive.h"

class MapReader {
public:
    MapReader() = default;
    ~MapReader() {
        for (Map* map : mMaps) {
            delete map;
        }
    }

    void parse(WadArchive::Lumps& lumps);

    using Maps = std::vector<Map*>;
    Maps& maps() {
        return mMaps;
    }

private:
    Maps mMaps;
    size_t mCursor=0;

    bool hasMore(WadArchive::Lumps& lumps) {
        return mCursor < lumps.size();
    }

    Lump& getNextLump(WadArchive::Lumps& lumps) {
        return lumps.at(mCursor++);
    }

    void parseMap(int episode, int id, WadArchive::Lumps& lumps);
};


#endif //WADEXPLORER_MAPREADER_H
