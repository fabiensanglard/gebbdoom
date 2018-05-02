//
// Created by Fabien Sanglard on 4/29/18.
//

#ifndef SPRITESHADER_PALETTE_H
#define SPRITESHADER_PALETTE_H


#include "Color.h"

class Palette {
public:
    Palette(const char* filename);

    Color& getColor(uint8_t index) {
        return getColor(0, index);
    }

    Color& getColor(uint8_t paletteID, uint8_t index) {
        return palette[paletteID][index];
    }

    bool isOk() {
        return ok;
    }

private:
    Color palette[14][256];
    bool ok = false;
};


#endif //SPRITESHADER_PALETTE_H
