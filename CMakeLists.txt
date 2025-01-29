cmake_minimum_required(VERSION 3.15)
project(Verdura)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/inc)
link_directories(${CMAKE_SOURCE_DIR}/lib)

include_directories(${CMAKE_SOURCE_DIR}/imgui)
include_directories(${CMAKE_SOURCE_DIR}/imgui/backends)
include_directories(${CMAKE_SOURCE_DIR}/dependencies_code)
include_directories( ${CMAKE_SOURCE_DIR}/SDL2-2.28.3/include)

# IMGUI static lib, creer
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

add_library("imgui" STATIC ${IMGUI_GLOB}) # fabriquer libimgui.a

# Linux
find_package(OpenGL REQUIRED)
# find_package(SDL2_mixer REQUIRED) # Optionnel

if(WIN32)
    set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/SDL2_mixer-devel-2.8.0-VC/SDL2_mixer-2.8.0/cmake
                          ${CMAKE_SOURCE_DIR}/SDL2-2.28.3/cmake
    )
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/MT")
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
add_executable(
        ${PROJECT_NAME} main.cpp
        dependencies_code/glad.c
        dependencies_code/stb.cpp
        Engine/Camera/Camera.h
        Engine/Mesh/OBJLoader.h
        Engine/Mesh/OBJMesh.h
        Engine/Shader/shader.h
        Engine/Shader/program.h
        Engine/Renderer/Renderer.h
        Engine/Mesh/MeshLoader.h
        Engine/Mesh/Mesh.h
        Engine/Mesh/MeshCache.h
        Engine/Common/Game.h
        Game/Character/Character.h
        Game/Character/Enemy.h
        Game/Character/Leveling.cpp
        Game/Character/Leveling.h
        Game/Character/Character.cpp
        Game/Character/Character.h
        Game/Character/Enemy.cpp
        Game/Character/Enemy.h
        Game/UI/GameUI.cpp
        Game/UI/GameUI.h
        Game/Ability/Attack.cpp
        Game/Ability/Attack.h
        Game/Character/Stats.cpp
        Game/Character/Stats.h
        Game/Ability/DefenseStrategy.cpp
        Game/Ability/DefenseStrategy.h
        Game/Ability/DamageReductionStrategy.cpp
        Game/Ability/DamageReductionStrategy.h
        Engine/Sound/SoundManager.h
        Engine/ParticleEffect/ParticleManager.h
        Game/Character/DamageManager.cpp
        Game/Character/DamageManager.h
        Game/UI/GameUI.h
        Game/UI/GameUI.cpp
        Game/Game/GameImpl.cpp
        Game/Game/GameImpl.h
        Engine/Events/EventManager.h
        Game/Events/HitEvent.h
        Game/Character/Hero.cpp
        Game/Character/Hero.h
        dependencies_code/imgui_image_loader_opengl.h
        Game/Events/UIInterruptEvent.h
        Game/Character/WarriorClass.cpp
        Game/Character/WarriorClass.h
        Engine/Mesh/OBJMeshFlyWeight.h
        Engine/Common/math_helper.h
        Game/Events/LevelingEvent.h
        Game/Events/GameEndEvent.h )
        
target_compile_definitions(Verdura PRIVATE GLM_ENABLE_EXPERIMENTAL)
target_include_directories( ${PROJECT_NAME} PUBLIC ${IMGUI_PATH}  ${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC imgui glfw3 ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES}
${CMAKE_SOURCE_DIR}/SDL2-2.28.3/lib/x64/SDL2.lib
${CMAKE_SOURCE_DIR}/SDL2_mixer-devel-2.8.0-VC/SDL2_mixer-2.8.0/lib/x64/SDL2_mixer.lib) 
if(WIN32)
add_custom_command(TARGET Verdura POST_BUILD


    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/SDL2-2.28.3/lib/x64
    $<TARGET_FILE_DIR:Verdura>

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/SDL2_mixer-devel-2.8.0-VC/SDL2_mixer-2.8.0/lib/x64
    $<TARGET_FILE_DIR:Verdura>)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_custom_command(
    TARGET Verdura POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${EXECUTABLE_OUTPUT_PATH}/assets
)