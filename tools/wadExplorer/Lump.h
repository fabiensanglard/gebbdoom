//
// Created by fabien sanglard on 2017-11-23.
//

#ifndef WADEXPLORER_LUMP_H
#define WADEXPLORER_LUMP_H


#include <string>

class Lump {
public:

    Lump(uint8_t *loc, uint8_t* base);

    std::string& name() {
        return mName;
    }

    size_t size() {
        return mSize;
    }

    uint8_t* payload() {
        return mPayload;
    }

private:
    std::string mName;
    size_t mSize;
    uint8_t* mPayload;
};


#endif //WADEXPLORER_LUMP_H
