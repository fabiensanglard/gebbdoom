//
// Created by fabien sanglard on 2017-11-23.
//

#include <iostream>
#include "Lump.h"

struct LumpHeader {
    int32_t offset;
    int32_t size;
    char name[8];
};

Lump::Lump(uint8_t* location, uint8_t* base) {
    LumpHeader* header = reinterpret_cast<LumpHeader*>(location);
    mName = std::string(header->name, 8);
    std::cout << mName << std::endl;
    mPayload = base + header->offset;
    mSize = header->size;
}
