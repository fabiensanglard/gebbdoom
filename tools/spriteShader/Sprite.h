//
// Created by Fabien Sanglard on 4/29/18.
//

#ifndef SPRITESHADER_SPRITE_H
#define SPRITESHADER_SPRITE_H


#include "Palette.h"
#include "Colormap.h"

class Sprite {
public:
    Sprite(const char* filename);
    ~Sprite();
    bool isOk() {
        return ok;
    }
    bool write(const char* filename, Palette& palette, Lightmap lightmap);
private:
    bool ok;
    uint8_t* data;
    uint8_t* mask;
    uint16_t width;
    uint16_t height;
    uint16_t xoffset;
    uint16_t yoffset;
};


#endif //SPRITESHADER_SPRITE_H
