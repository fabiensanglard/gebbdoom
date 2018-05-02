#include <iostream>

#include "Palette.h"
#include "Colormap.h"
#include "Sprite.h"

void generateShades(Sprite &sprite, Palette &palette, Colormap &colormap) {

}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage:" << argv[0] << " [spritePath].tga" << std::endl;
        return EXIT_FAILURE;
    }

    Palette palette("PLAYPAL.lump");
    if (!palette.isOk()) {
        return EXIT_FAILURE;
    }

    Colormap colormap("COLORMAP.lmp");
    if (!colormap.isOk()) {
        return EXIT_FAILURE;
    }


    Sprite sprite(argv[1]);
    if (!sprite.isOk()) {
        return EXIT_FAILURE;
    }

    // Generate all 34 shades of the sprites.
    for (int i=0 ; i < 32 ; i++) {
        std::string f = "cacodemon" ;
        f += std::to_string(i+1);
        f += ".tga";
        sprite.write(f.c_str(), palette, colormap.getLightMap(i));
    }

    return EXIT_SUCCESS;
}