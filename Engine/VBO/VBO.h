//
// Created by hous on 11/7/24.
//

#ifndef VBO_H
#define VBO_H
#include <cstddef> // std offsetof


class VBO {
public:
    VBO();
    void BindVBO();
    void UnBindVBO();
    void LinkAttribute();
};



#endif //VBO_H
