//
// Created by Fabien Sanglard on 4/29/18.
//

#include <iostream>
#include <cstdio>
#include "Palette.h"

Palette::Palette(const char* filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        std::cout << "Unable to open " << filename << std::endl;
        return;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (size != 768 * 14) {
        std::cout << "File " << filename << "size is " << size << ", expected" <<  768 * 14 << "." << std::endl;
        return;
    }

    uint8_t* buffer = (uint8_t*) malloc(size);
    fread(buffer, size, 1, f);
    fclose(f);

    uint8_t* cursor = buffer;
    for (int p = 0 ;p < 14 ; p++) {
        for (int i = 0; i < 256; i++, cursor += 3) {
            palette[p][i] = {*cursor, *(cursor + 1), *(cursor + 2)};
        }
    }

    free(buffer);
    ok = true;
}
