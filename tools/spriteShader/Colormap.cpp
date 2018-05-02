//
// Created by Fabien Sanglard on 4/29/18.
//

#include <cstdio>
#include <iostream>
#include "Colormap.h"

Colormap::Colormap(const char* filename) {

    FILE *f = fopen(filename, "rb");
    if (!f) {
        std::cout << "Unable to open " << filename << std::endl;
        return;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (size != 34*256) {
        std::cout << "File " << filename << "size is " << size << ", expected" << (34 * 256) << "." << std::endl;
        return;
    }

    uint8_t* buffer = (uint8_t*) malloc(size);
    fread(buffer, size, 1, f);
    fclose(f);

    uint8_t* cursor = buffer;
    for (size_t y = 0 ; y < 34 ; y++) {
        for (size_t x = 0 ; x < 256 ; x++) {
            lightmaps[y][x] = *cursor++;
        }
    }
    free(buffer);
    ok = true;
}
