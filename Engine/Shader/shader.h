//
// Created by hous on 12/2/24.
//

#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <glad.h>
#include <iostream>

#include "error_codes.h"


class Shader {

    std::string shaderSource;
    std::string shaderSourceFilePath;
    GLuint shaderID{0};

    public:
    std::string shader_source() const {
        return shaderSource;
    }

    GLuint shader_id() const {
        return shaderID;
    }
    void set_shader_source(const std::string& _shaderSource) {
        shaderSource = _shaderSource;
    }
    std::string shader_source_file_path() const {
        return shaderSourceFilePath;
    }
    void set_shader_source_file_path(const std::string &shader_source_file_path) {
        shaderSourceFilePath = shader_source_file_path;
    }

    static bool loadShaderToString(const char* sourcePath,
                                   std::string& dest_string)
    {
        std::ifstream f(sourcePath);
        bool success = false;
        if(f.is_open()){
            std::string line;
            while(getline(f,line))
            {
                dest_string.append(line);
                dest_string.append("\n");
            }
            success = true;
        }
        else
            success = false;
        return success;
    }

    void compileShader(const GLenum shaderType) {
        shaderID = glCreateShader(shaderType);
        const char* sourceCstr = shaderSource.c_str();// we need a lvalue for & operator
        glShaderSource(shaderID, 1, &sourceCstr, nullptr);
        glCompileShader(shaderID);
    }

    static void checkShader(GLuint shaderID) {
        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
            std::cerr << "ERROR: SHADER COMPILING FAILED" << infoLog << std::endl;
            glDeleteShader(shaderID);
            exit(SHADER_COMPILE_ERROR);
        }
    }
    Shader(std::string const& _shaderFilePath,
           GLenum shaderType)
    {
        loadShaderToString(_shaderFilePath.c_str(),shaderSource);
        compileShader(shaderType);
        checkShader(shaderID);

    }

    ~Shader()
    {
        glDeleteShader(shaderID);
    }
};
#endif //SHADER_H
