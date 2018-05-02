//
// Created by Fabien Sanglard on 4/29/18.
//

#ifndef SPRITESHADER_COLORMAP_H
#define SPRITESHADER_COLORMAP_H


#include "Palette.h"

using Lightmap = uint8_t*;

class Colormap {
public:
    Colormap(const char* filename);

    Lightmap getLightMap(uint8_t lightmap) {
        return lightmaps[lightmap];
    }

    bool isOk() {
        return ok;
    }

private:
    uint8_t lightmaps[34][256];
    bool ok = false;
};


#endif //SPRITESHADER_COLORMAP_H
