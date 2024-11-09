//
// Created by hous on 11/8/24.
//

#ifndef DATA_TYPES_H
#define DATA_TYPES_H
#include "../dependencies_code/glad.h"

#ifdef VERDURA_USE_INT_GLID
    typedef GLint Glid;
#else
    typedef GLuint Glid;
#endif

#define VERDURA_INVALID_GL_ID (-1)

#endif //DATA_TYPES_H
