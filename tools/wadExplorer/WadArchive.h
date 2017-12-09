//
// Created by fabien sanglard on 2017-11-23.
//

#ifndef WADEXPLORER_WADARCHIVE_H
#define WADEXPLORER_WADARCHIVE_H

#include <vector>

#include "Lump.h"

class WadArchive {
public:
    WadArchive() = default;

    std::vector<Lump>& lumps() {
        return mLumps;
    }

    void parse(uint8_t * wad);

    using Lumps = std::vector<Lump>;
private:
    Lumps mLumps;
    std::string mMagic;
};


#endif //WADEXPLORER_WADARCHIVE_H
