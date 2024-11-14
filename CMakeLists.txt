cmake_minimum_required(VERSION 3.29)
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

        # specific bindings, if you use Direct3D or Vulkan, change this...
        ${IMGUI_PATH}/backends/imgui_impl_glfw.h
        ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.h
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.cpp)

add_library("imgui" STATIC ${IMGUI_GLOB}) # build a libimgui.a

# Linux
find_package(OpenGL REQUIRED)

add_executable(
        ${PROJECT_NAME} main.cpp
        dependencies_code/glad.c
        dependencies_code/stb.cpp
        Engine/VBO/VBO.cpp
        Engine/VBO/VBO.h
        Engine/Texture/Texture.cpp
        Engine/Texture/Texture.h
        Engine/ShaderManager/ShaderManager.cpp
        Engine/ShaderManager/ShaderManager.h
        Engine/Camera/Camera.cpp
        Engine/Camera/Camera.h
        Engine/Light/BaseLight/BaseLight.cpp
        Engine/Light/BaseLight/BaseLight.h
        Engine/Light/AmbientLight/AmbientLight.cpp
        Engine/Light/AmbientLight/AmbientLight.h
        Engine/Light/DirectionalLight/DirectionalLight.cpp
        Engine/Light/DirectionalLight/DirectionalLight.h
        Helpers/data_types.h
        Engine/Mesh/Mesh.cpp
        Engine/Mesh/Mesh.h
        GLBHandlers/GLBMesh/GLBMesh.cpp
        GLBHandlers/GLBMesh/GLBMesh.h
        GLBHandlers/GLBLoader/glb_loader.h
)

target_include_directories( ${PROJECT_NAME} PUBLIC ${IMGUI_PATH})
target_link_libraries(${PROJECT_NAME} PUBLIC imgui glfw ${OPENGL_LIBRARIES}) # windows glfw3, avec un 3, linux sans 3
