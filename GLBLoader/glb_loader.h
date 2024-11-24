//
// Created by hous on 11/14/24.
//

#ifndef GLB_LOADER_H
#define GLB_LOADER_H

// Credits : Réalisé grâce à la lib 'tiny_gltf' pour me permettre d'aller plus vite
// sur le principal : Le jeu
#include <glm/glm.hpp>
#include <glad.h>
#include <iostream>
#include <tiny_gltf.h>
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


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
inline void bindMesh(std::map<int, GLuint>& vbos,
                     tinygltf::Model &model, tinygltf::Mesh &mesh) {
  for (size_t i = 0; i < model.bufferViews.size(); ++i) {
    const tinygltf::BufferView &bufferView = model.bufferViews[i];
    if (bufferView.target == 0) {  // TODO impl drawarrays
      std::cout << "WARN: bufferView.target is zero" << std::endl;
      continue;
    }
    const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
    std::cout << "bufferview.target " << bufferView.target << std::endl;
    GLuint vbo;
    glGenBuffers(1, &vbo);
    vbos[i] = vbo;
    glBindBuffer(bufferView.target, vbo);
    std::cout << "buffer.data.size = " << buffer.data.size()
              << ", bufferview.byteOffset = " << bufferView.byteOffset
              << std::endl;
    glBufferData(bufferView.target, bufferView.byteLength,
                 &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
  }
  for (const auto& primitive : mesh.primitives) {
    tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

    for (auto &attrib : primitive.attributes) {
      tinygltf::Accessor accessor = model.accessors[attrib.second];
      int byteStride =
          accessor.ByteStride(model.bufferViews[accessor.bufferView]);
      glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

      int size = 1;
      if (accessor.type != TINYGLTF_TYPE_SCALAR) {
        size = accessor.type;
      }

      int vaa = -1;
      if (attrib.first == "POSITION") vaa = 0;
      if (attrib.first == "NORMAL") vaa = 1;
      if (attrib.first == "TEXCOORD_0") vaa = 2;
      if (vaa > -1) {
        glEnableVertexAttribArray(vaa);
        glVertexAttribPointer(vaa, size, accessor.componentType,
                              accessor.normalized ? GL_TRUE : GL_FALSE,
                              byteStride, BUFFER_OFFSET(accessor.byteOffset));
      } else
        std::cout << "vaa missing: " << attrib.first << std::endl;
    }

    if (!model.textures.empty()) {
      // fixme: Use material's baseColor
      tinygltf::Texture &tex = model.textures[0];

      if (tex.source > -1) {

        GLuint texid;
        glGenTextures(1, &texid);

        tinygltf::Image &image = model.images[tex.source];

        glBindTexture(GL_TEXTURE_2D, texid);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        GLenum format = GL_RGBA;

        if (image.component == 1) {
          format = GL_RED;
        } else if (image.component == 2) {
          format = GL_RG;
        } else if (image.component == 3) {
          format = GL_RGB;
        }
        GLenum type = GL_UNSIGNED_BYTE;
        if (image.bits == 8) {
        } else if (image.bits == 16) {
          type = GL_UNSIGNED_SHORT;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0,
                     format, type, &image.image.at(0));
      }
    }
  }
}

// bind models
inline void bindModelNodes(std::map<int, GLuint>& vbos, tinygltf::Model &model,
                           tinygltf::Node &node) {
  if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
    bindMesh(vbos, model, model.meshes[node.mesh]);
  }

  for (int i : node.children) {
    assert((i >= 0) && (i < model.nodes.size()));
    bindModelNodes(vbos, model, model.nodes[i]);
  }
}

inline std::pair<GLuint, std::map<int, GLuint>> bindModel(tinygltf::Model &model) {
  std::map<int, GLuint> vbos;
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  const tinygltf::Scene &scene = model.scenes[model.defaultScene];
  for (int node : scene.nodes) {
    assert((node >= 0) && (node < model.nodes.size()));
    bindModelNodes(vbos, model, model.nodes[node]);
  }

  glBindVertexArray(0);
  // cleanup vbos but do not delete index buffers yet
  for (auto it = vbos.cbegin(); it != vbos.cend();) {
    tinygltf::BufferView bufferView = model.bufferViews[it->first];
    if (bufferView.target != GL_ELEMENT_ARRAY_BUFFER) {
      glDeleteBuffers(1, &vbos[it->first]);
      vbos.erase(it++);
    }
    else {
      ++it;
    }
  }

  return {vao, vbos};
}

inline void drawMesh(const std::map<int, GLuint>& vbos,
                     tinygltf::Model &model, tinygltf::Mesh &mesh) {
  for (const auto& primitive : mesh.primitives) {
    tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos.at(indexAccessor.bufferView));
    glDrawElements(primitive.mode, indexAccessor.count,
                   indexAccessor.componentType,
                   BUFFER_OFFSET(indexAccessor.byteOffset));
  }
}

// recursively draw node and children nodes of model
inline void drawModelNodes(const std::pair<GLuint, std::map<int, GLuint>>& vaoAndEbos,
                           tinygltf::Model &model, tinygltf::Node &node) {
  if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
    drawMesh(vaoAndEbos.second, model, model.meshes[node.mesh]);
  }
  for (int i : node.children) {
    drawModelNodes(vaoAndEbos, model, model.nodes[i]);
  }
}

inline void drawModel(const std::pair<GLuint, std::map<int, GLuint>>& vaoAndEbos,
                      tinygltf::Model &model) {
  glBindVertexArray(vaoAndEbos.first);

  const tinygltf::Scene &scene = model.scenes[model.defaultScene];
  for (size_t i = 0; i < scene.nodes.size(); ++i) {
    drawModelNodes(vaoAndEbos, model, model.nodes[scene.nodes[i]]);
  }

  glBindVertexArray(0);
}

inline void dbgModel(tinygltf::Model &model) {
  for (auto &mesh : model.meshes) {
    std::cout << "mesh : " << mesh.name << std::endl;
    for (auto &primitive : mesh.primitives) {
      const tinygltf::Accessor &indexAccessor =
          model.accessors[primitive.indices];

      std::cout << "indexaccessor: count " << indexAccessor.count << ", type "
                << indexAccessor.componentType << std::endl;

      tinygltf::Material &mat = model.materials[primitive.material];
      for (auto &mats : mat.values) {
        std::cout << "mat : " << mats.first.c_str() << std::endl;
      }

      for (auto &image : model.images) {
        std::cout << "image name : " << image.uri << std::endl;
        std::cout << "  size : " << image.image.size() << std::endl;
        std::cout << "  w/h : " << image.width << "/" << image.height
                  << std::endl;
      }

      std::cout << "indices : " << primitive.indices << std::endl;
      std::cout << "mode     : "
                << "(" << primitive.mode << ")" << std::endl;

      for (auto &attrib : primitive.attributes) {
        std::cout << "attribute : " << attrib.first.c_str() << std::endl;
      }
    }
  }
}

#endif //GLB_LOADER_H
