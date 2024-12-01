cmake_minimum_required(VERSION 3.15)
project(Verdura)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/inc)
link_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/imgui)
include_directories(${CMAKE_SOURCE_DIR}/imgui/backends)
include_directories(${CMAKE_SOURCE_DIR}/dependencies_code)

# IMGUI static library creation
set(IMGUI_PATH ${CMAKE_SOURCE_DIR}/imgui)
file(GLOB IMGUI_GLOB
        ${IMGUI_PATH}/imgui.h
        ${IMGUI_PATH}/imgui.cpp
        ${IMGUI_PATH}/imconfig.h
        ${IMGUI_PATH}/imgui_demo.cpp
        ${IMGUI_PATH}/imgui_draw.cpp
        ${IMGUI_PATH}/imgui_internal.h
        ${IMGUI_PATH}/imstb_rectpack.h
        ${IMGUI_PATH}/imstb_textedit.h
        ${IMGUI_PATH}/imstb_truetype.h
        ${IMGUI_PATH}/imgui_tables.cpp
        ${IMGUI_PATH}/imgui_widgets.cpp

        # Les fichiers spcifiques à OpenGL
        ${IMGUI_PATH}/backends/imgui_impl_glfw.h
        ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.h
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.cpp)

add_library("imgui" STATIC ${IMGUI_GLOB}) # build libimgui.a

# Linux
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

add_executable(
        ${PROJECT_NAME} main.cpp
        dependencies_code/glad.c
        dependencies_code/stb.cpp
        Engine/Texture/Texture.h
        Engine/ShaderManager/ShaderManager.h
        Engine/Camera/Camera.h
        Engine/Mesh/OBJLoader.h
        Engine/Mesh/OBJMesh.h
)

target_include_directories( ${PROJECT_NAME} PUBLIC ${IMGUI_PATH})
target_link_libraries(${PROJECT_NAME} PUBLIC imgui glfw ${OPENGL_LIBRARIES} assimp::assimp) # windows 'glfw3', avec un 3, linux sans 3
