//
// Created by hous on 11/14/24.
//

#ifndef GLB_LOADER_H
#define GLB_LOADER_H

#include <iostream>
#include <tiny_gltf.h>

inline bool loadModel(tinygltf::Model &model, const char *filename) {
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    // bool res = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
    bool res = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cout << "ERR: " << err << std::endl;
    }

    if (!res)
        std::cout << "Failed to load glTF: " << filename << std::endl;
    else
        std::cout << "Loaded glTF: " << filename << std::endl;

    return res;
}


#endif //GLB_LOADER_H
