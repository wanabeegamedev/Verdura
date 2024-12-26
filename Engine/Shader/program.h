//
// Created by hous on 12/2/24.
//

#ifndef PROGRAM_H
#define PROGRAM_H
#include <string>
#include <fstream>
#include <glad.h>
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "program.h"
#include "error_codes.h"

class Program {

    GLuint programID{};
    std::string programName{};
    GLuint currentVertexShaderID{};
    GLuint currentFragmentShaderID{};
    std::unordered_map<std::string, GLint> uniformCache;//TODO use addUniform to add the uniforms from the shaders
    public:

    void checkProgram() const {
        GLint success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(programID, 512, nullptr, infoLog);
            std::cerr << "ERROR: PROGRAM LINKING FAILED\n" << infoLog << std::endl;
            exit(PROGRAM_LINK_ERROR);
        }
    }

    void setName(std::string const& name) {
        programName = name;
    };

    Program(
        GLuint current_vertex_shader_id,
        GLuint current_fragment_shader_id)
        : currentVertexShaderID(current_vertex_shader_id),
          currentFragmentShaderID(current_fragment_shader_id)

    {
        programID = glCreateProgram();
        glAttachShader(programID, current_vertex_shader_id);
        glAttachShader(programID, current_fragment_shader_id);
        glLinkProgram(programID);
        checkProgram();
        glDeleteShader(currentFragmentShaderID);
        glDeleteShader(currentVertexShaderID);
        // TODO in the main , call the shaders Destructors
    }
    void bind() const
    {
        glUseProgram(programID);
    }


    GLint getUniformLocation(const std::string& name) {
        if (uniformCache.find(name) != uniformCache.end()) {
            return uniformCache[name];
        }

        GLint location = glGetUniformLocation(programID, name.c_str());
        if (location == -1) {
            std::cerr << "WARNING: Uniform '" << name;// << "' doesn't exist in program ID " << programID << "!" << std::endl;
        }
        uniformCache[name] = location;
        return location;
    }
    void setUniform1f(const std::string& name, float value) {
        GLint location = getUniformLocation(name);
        if (location != -1) {
            glUniform1f(location, value);
        }
    }
    void setUniform1i(const std::string& name, int value) {
        GLint location = getUniformLocation(name);
        if (location != -1) {
            glUniform1i(location, value);
        }
    }
    void setUniform3f(const std::string& name, const glm::vec3& value) {
        GLint location = getUniformLocation(name);
        if (location != -1) {
            glUniform3f(location, value.x, value.y, value.z);
        }
    }

    void setUniformMat4(const std::string& name, const glm::mat4& value) {
        GLint location = getUniformLocation(name);
        if (location != -1) {
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
        }
    }

    GLuint program_id() const {
        return programID;
    }
    std::string program_name() const {
        return programName;
    }
    GLuint current_vertex_shader_id() const {
        return currentVertexShaderID;
    }
    void set_current_vertex_shader_id(GLuint current_vertex_shader_id) {
        currentVertexShaderID = current_vertex_shader_id;
    }
    GLuint current_fragment_shader_id() const {
        return currentFragmentShaderID;
    }
    void set_current_fragment_shader_id(GLuint current_fragment_shader_id) {
        currentFragmentShaderID = current_fragment_shader_id;
    }

    ~Program()
    {
        glDeleteProgram(programID);
    }
};

#endif //PROGRAM_H
