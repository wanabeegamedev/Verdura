//
// Created by hous on 12/30/24.
//

#include "Hero.h"

Hero::Hero(OBJMesh& _characterMesh,Stats& _stats, Inventory& _inventory,
        const std::string& _name) {
        characterMesh = _characterMesh;
        stats = _stats;
        inventory = _inventory;
        name = _name;
        //TODO seed HeroClasses
}

Hero::~Hero() = default;
