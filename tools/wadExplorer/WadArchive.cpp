//
// Created by fabien sanglard on 2017-11-23.
//
#include "WadArchive.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <cassert>
#include <fcntl.h>
#include <zconf.h>
#include <iostream>

struct WadHeader{
    char magic[4];
    int32_t numLumps;
    int32_t offsetToLumps;
};



void WadArchive::parse(uint8_t * wad) {

    WadHeader* header = reinterpret_cast<WadHeader*>(wad);

    mMagic = std::string(header->magic, 4);

    uint8_t* rover = static_cast<uint8_t*>(wad)+header->offsetToLumps;
    for(int i=0 ; i < header->numLumps ; i++) {
        mLumps.push_back(Lump(rover, wad));
        rover += 16;
    }



}
