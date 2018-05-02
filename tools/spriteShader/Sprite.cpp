//
// Created by Fabien Sanglard on 4/29/18.
//

#include <cstdio>
#include <iostream>
#include "Sprite.h"
#include "targa.h"

Sprite::Sprite(const char* filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        std::cout << "Unable to open " << filename << std::endl;
        return;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);


    uint8_t* buffer = (uint8_t*) malloc(size);
    fread(buffer, size, 1, f);
    fclose(f);

    uint16_t* cursor = reinterpret_cast<uint16_t *>(buffer);

    width  = *cursor++;
    height = *cursor++;
    xoffset  = *cursor++;
    yoffset  = *cursor++;

    mask = static_cast<uint8_t *>(calloc(width * height, 1));
    data = static_cast<uint8_t *>(calloc(width * height, 1));
    for(int x = 0 ; x < width ; x++) {
        uint32_t postOffset = *(uint32_t*)cursor;
        cursor += 2;
        uint8_t* postData = buffer + postOffset;

        // Read post.
        uint8_t rowstart;
        do {
            rowstart = *postData++;
            if (rowstart == 255) // end of post
                break;

            uint8_t pixelcount = *postData++;
            for (int i = 0; i < pixelcount; i++) {
                data[x + rowstart * width + i * width] = *postData++;
                mask[x + rowstart * width + i * width] = 1;
            }
            postData++;
            postData++;
        } while (rowstart != 255);

    }

    free(buffer);
    ok = true;
}

Sprite::~Sprite() {
    if (ok) {
        free(data);
        free(mask);
    }
}

bool Sprite::write(const char *filename, Palette &palette, Lightmap lightmap) {
    const uint8_t border = 2;
    uint8_t* bgr = static_cast<uint8_t *>(calloc((width+border*2) * (height+border*2) * 4, 1));
    for (int y=0 ; y < height ; y++) {
        uint8_t* cursor = bgr + border * 4 + (y+1) * (width + (border*2)) * 4;
        for (int x=0 ; x < width; x++) {
            uint8_t index = lightmap[data[x + y * width]];
            Color c = palette.getColor(index);
            *cursor++ = c.b;
            *cursor++ = c.g;
            *cursor++ = c.r;
            *cursor++ = mask[x + y * width] == 1 ? 255 : 0;
        }
    }
    tga_result r = tga_write_bgr(filename, bgr, width+border*2 , height+border*2, 32);

    free(bgr);
    return true;
}
