//
// Created by fabien sanglard on 2017-12-24.
//

#include "Things.h"

std::unordered_map<int, ThingDef> thingsDefs = {
    // Monsters
    {68, {2, 64, "BSPI", Class::Monster}},  // Arachnotron
    {64, {2, 20, "VILE", Class::Monster}}, // Arch-Vile
    {3003, {2, 24, "BOSS", Class::Monster}}, // Baron of Hell
    {3005, {2, 31, "HEAD", Class::Monster}}, // Cacodemon
    {65, {2, 20, "CPOS", Class::Monster}}, // Chaingunner
    {72, {2, 16, "KEEN", Class::Monster}}, // Commander Keen
    {16, {2, 40, "CYBR", Class::Monster}}, // Cyberdemon
    {3002, {2, 30, "SARG", Class::Monster}}, // Demon
    {3004, {2, 20, "POSS", Class::Monster}}, // Former Human Trooper (MT_POSSESSED)
    {9, {2, 20, "SPOS", Class::Monster}}, // Sergeant
    {69, {2, 24, "BOS2", Class::Monster}}, // Hell Knight
    {3001, {2, 20, "TROO", Class::Monster}}, // Imp
    {3006, {2, 16, "SKUL", Class::Monster}}, // Lost Soul
    {67, {2, 48, "FATT", Class::Monster}}, // Mancubus
    {71, {2, 31, "PAIN", Class::Monster}}, // Pain Elemental
    {66, {2, 20, "SKEL", Class::Monster}}, // Revenant
    {58, {2, 30, "SARG", Class::Monster}}, // Spectre
    {7, {2, 128, "SPID", Class::Monster}}, // Spider Mastermind
    {84, {2, 20, "SSWV", Class::Monster}},  // Wolfenstein SS

    // Human spawn
    {1 , {2, 16, "PLAY", Class::PlayerSpawn }},
    {2 , {2, 16, "PLAY", Class::PlayerSpawn }},
    {3 , {2, 16, "PLAY", Class::PlayerSpawn }},
    {4 , {2, 16, "PLAY", Class::PlayerSpawn }},
    {11, {2, 16, "PLAY", Class::PlayerSpawn }},

    // Health

};

bool isMonster(size_t type) {
    if (thingsDefs.find(type) == thingsDefs.end()) {
        return false;
    }
    return thingsDefs[type].klass == Class::Monster;
}

bool isPlayer(size_t type) {
    if (thingsDefs.find(type) == thingsDefs.end()) {
        return false;
    }
    return thingsDefs[type].klass == Class::PlayerSpawn;
}
