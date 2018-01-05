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

struct WadHeader {
    char magic[4];
    int32_t numLumps;
    int32_t offsetToLumps;
};


WadArchive::WadArchive(uint8_t *data, size_t size) : mData(data), mSize(size) {
}

void WadArchive::parse() {
    WadHeader *header = reinterpret_cast<WadHeader *>(mData);
    mMagic = std::string(header->magic, 4);

    uint8_t *rover = static_cast<uint8_t *>(mData) + header->offsetToLumps;
    for (int i = 0; i < header->numLumps; i++) {
        mLumps.push_back(Lump(rover, mData));
        rover += 16;
    }


}

void WadArchive::categories() {
    for (Lump lump : mLumps) {
        Category cat = categorizeLump(lump);
        categoriesSize[cat] += lump.size();
    }

    for(size_t i = 0 ; i < Category::NUM_CATEGORIES ; i++) {
        std::cout << i << ":" << categoriesSize[i] << "(" << categoriesSize[i]/(float)mSize * 100 << "%)" << std::endl;
    }
}

WadArchive::Category WadArchive::categorizeLump(Lump &lump) {

    // We are withing a map.
    if (catMode == MAP) {
        if (!strcmp(lump.name().c_str(), "BLOCKMAP")) { // Last lump in a map.
            catMode = OTHERS;
        }
        return MAP;
    }
    std::cout << lump.name() << std::endl;
    if (lump.name().size() >= 2 && lump.name()[0] == 'E' && lump.name()[2] == 'M') {
        catMode = MAP;
        return MAP;
    }
    else if(lump.name().size() >= 3 && !strncmp(lump.name().c_str(), "MAP", 3)) {
        catMode = MAP;
        return MAP;
    }

    if (!strncmp(lump.name().c_str(), "DS", 2)) { // PCM Sound
        return Category::SOUND;
    }
    if (!strncmp(lump.name().c_str(), "DP", 2)) { // PC Speaker
        return Category::SOUND;
    }

    if (!strncmp(lump.name().c_str(), "D_", 2)) {
        return Category::MUSIC;
    }

    if (!strcmp(lump.name().c_str(), "PLAYPAL")) {
        return Category::OTHERS;
    }

    if (!strcmp(lump.name().c_str(), "COLORMAP")) {
        return Category::OTHERS;
    }

    if (!strcmp(lump.name().c_str(), "ENDOOM")) {
        return Category::OTHERS;
    }

    if (!strcmp(lump.name().c_str(), "TEXTURE1")) {
        return Category::OTHERS;
    }

    if (!strcmp(lump.name().c_str(), "TEXTURE2")) {
        return Category::OTHERS;
    }
    if (!strcmp(lump.name().c_str(), "PNAMES")) {
        return Category::OTHERS;
    }
    if (!strcmp(lump.name().c_str(), "GENMIDI")) {
        return Category::MUSIC;
    }
    if (!strcmp(lump.name().c_str(), "DMXGUS")) {
        return Category::MUSIC;
    }

    if (!strncmp(lump.name().c_str(), "DEMO", 4)) {
        return Category::OTHERS;
    }


    return GFX;
}

