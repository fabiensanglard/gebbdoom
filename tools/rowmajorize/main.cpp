#include <iostream>

#include <sys/stat.h>

using namespace std;

typedef struct TgaHeader {
    char  idlength;
    char  colourmaptype;
    char  datatypecode;
    short int colourmaporigin;
    short int colourmaplength;
    char  colourmapdepth;
    short int x_origin;
    short int y_origin;
    short width; // This is not portable (little endian only)
    short height;// same here.
    char  bitsperpixel;
    char  imagedescriptor;
} __attribute__((packed));

typedef struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

int main(int argc, char** argv) {

    if (argc < 2) {
        cerr << "Missing input filename" << endl;
        return EXIT_FAILURE;
    }

    // load TGA file.
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        cerr << "Unable to open file " << argv[1] << endl;
        return EXIT_FAILURE;
    }

    if (sizeof(TgaHeader) != 18) {
        cerr << "Aligment error, TgaHeader should be 18 bytes long." <<  endl;
        return EXIT_FAILURE;
    }

    TgaHeader header;
    int read = fread(&header, sizeof(header), 1, file);
    if (!read) {
        cerr << "Unable to read header." << endl;
        return EXIT_FAILURE;
    }

    if (header.bitsperpixel != 24) {
        cerr << "Only 24-bit TGA are supported (got: " << header.bitsperpixel << ")." << endl;
        return EXIT_FAILURE;
    }

    size_t size = (size_t) header.width * header.height * header.bitsperpixel/8;

    Pixel* src = (Pixel*)malloc(size); // Leak it, I don't care. I am a rebel. Yea. So badass.
    Pixel* dst = (Pixel*)malloc(size); // Leak it, I don't care. I am a rebel. Yea. So badass.
    fread(src, size, 1, file);
    fclose(file);

    // Rowmajorize
    for(int y=0; y<header.height; y++)
        for(int x=0; x<header.width; x++)
            dst[x*header.height+y] = src[y*header.width+x];

    // write TGA file.
    FILE* out = fopen("out.tga", "w");
    if (!out) {
        cerr << "Unble to create output file." << endl;
        return EXIT_FAILURE;
    }
    fwrite(&header, sizeof(header), 1, out);
    fwrite(dst, size, 1, out);
    fclose(out);

    // I am weak.
    free(src);
    free(dst);

    return EXIT_SUCCESS;
}