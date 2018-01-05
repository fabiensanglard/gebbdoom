//
// Created by fabien sanglard on 2017-11-23.
//

#ifndef WADEXPLORER_WADARCHIVE_H
#define WADEXPLORER_WADARCHIVE_H

#include <vector>
#include <array>

#include "Lump.h"

class WadArchive {
public:
    WadArchive(uint8_t* data, size_t size);

    std::vector<Lump>& lumps() {
        return mLumps;
    }

    void parse();
    void categories();

    using Lumps = std::vector<Lump>;
private:
    Lumps mLumps;
    std::string mMagic;
    uint8_t* mData;
    size_t mSize;

    enum Category {MAP, GFX, SOUND, MUSIC, OTHERS, NUM_CATEGORIES};
    std::array<size_t, NUM_CATEGORIES> categoriesSize = {0, 0, 0, 0, 0};
    Category catMode = OTHERS;

    Category categorizeLump(Lump &lump);
};


#endif //WADEXPLORER_WADARCHIVE_H
