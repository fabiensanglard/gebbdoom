#include <iostream>
#include <stdlib.h>
#include <zconf.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "WadArchive.h"
#include "Lump.h"
#include "MapReader.h"
#include "SVGRenderer.h"

size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

//int fd;
//size_t filesize;

uint8_t* openArchive(const char* filepath) {

    size_t filesize = getFilesize(filepath);

    //Open file
    int fd = open(filepath, O_RDONLY, 0);
    assert(fd != -1);
    //Execute mmap
    void* wad = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);

    return static_cast<uint8_t*>(wad);
}

//void closeArchive(void* wad) {
//    //Cleanup
//    int rc = munmap(wad, filesize);
//    assert(rc == 0);
//    close(fd);
//}

int main(int argc, char** argv) {

//    Point planStart = {-5, 0};
//    Point planEnd = {5,0};
//    Plan p(planStart, planEnd);
//    Point start = {2, 1};
//    Point end = {2,-1};
//    Point intersection = p.findIntersection(start, end);
//    assert(intersection.x == 2);
//    assert(intersection.y == 0);

    uint8_t* wad = openArchive(argv[1]);

    WadArchive archive;
    archive.parse(wad);

    MapReader mapReader;
    mapReader.parse(archive.lumps());

    SVGRenderer svgRenderer;
    svgRenderer.render(mapReader.maps());
//    closeArchive(wad);

    return EXIT_SUCCESS;
}
