//
// Created by fabien sanglard on 2017-12-24.
//

#ifndef WADEXPLORER_THINGS_H
#define WADEXPLORER_THINGS_H

#include <cstdint>
#include <unordered_map>
#include <string>

#define THING_LEVEL1_2         0x0001
#define THING_LEVEL3           0x0002
#define THING_LEVEL4_5         0x0004
#define THING_DEAF             0x0008
#define THING_NO_SINGLE_PLAYER 0x0010

enum Class {Artifact, Pickup, Weapon, Monster, Obstacle, Hang, PlayerSpawn};

struct ThingDef {
    ThingDef(): sprite("") {
    }

    ThingDef(uint8_t version, uint8_t radius, std::string sprite, Class k) :
            version(version), radius(radius), sprite(sprite), klass(k) {}
    uint8_t version;
    uint8_t radius;
    std::string sprite;
    Class klass;
};

bool isMonster(size_t type);
bool isPlayer(size_t type);
extern std::unordered_map<int,ThingDef> thingsDefs;

#endif //WADEXPLORER_THINGS_H
