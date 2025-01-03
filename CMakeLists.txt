cmake_minimum_required(VERSION 3.15)
project(Verdura)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/inc)
link_directories(${CMAKE_SOURCE_DIR}/lib)
link_directories(${SDL2_MIXER_LIB_DIRS})
link_directories(${SDL2_LIBRARIES_DIRS})

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
find_package(SDL2_mixer REQUIRED) # Optional, if using SDL2_mixer


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
        Game/Inventory/Inventory.h
        Game/Character/Leveling.cpp
        Game/Character/Leveling.h
        Game/Character/Character.cpp
        Game/Character/Character.h
        Game/Inventory/Inventory.cpp
        Game/Inventory/Inventory.h
        Game/Character/Enemy.cpp
        Game/Character/Enemy.h
        Game/UI/GameUI.cpp
        Game/UI/GameUI.h
        Game/Inventory/Shop.cpp
        Game/Inventory/Shop.h
        Game/Inventory/Item.cpp
        Game/Inventory/Item.h
        Game/Ability/Ability.cpp
        Game/Ability/Ability.h
        Game/Ability/Attack.cpp
        Game/Ability/Attack.h
        Game/Character/CharacterMovement.cpp
        Game/Character/CharacterMovement.h
        Game/Character/Stats.cpp
        Game/Character/Stats.h
        Game/Ability/DefenseStrategy.cpp
        Game/Ability/DefenseStrategy.h
        Game/Ability/RetourALEnvoyeurStrategy.cpp
        Game/Ability/RetourALEnvoyeurStrategy.h
        Game/Ability/DamageReductionStrategy.cpp
        Game/Ability/DamageReductionStrategy.h
        Engine/Sound/SoundManager.h
        Engine/ParticleEffect/ParticleManager.h
        Game/Character/DamageManager.cpp
        Game/Character/DamageManager.h
        Game/UI/GameUI.h
        Game/UI/GameUI.cpp
        Engine/Physics/Movement.h
        Game/Game/GameImpl.cpp
        Game/Game/GameImpl.h
        Engine/Events/EventManager.h
        Game/Events/HitEvent.h
        Game/Events/UseOrBuyItemEvent.h
        Game/Character/Hero.cpp
        Game/Character/Hero.h
        dependencies_code/imgui_image_loader_opengl.h
        Game/Events/SoundRequestEvent.h
        Game/Events/UIInterruptEvent.h
        Game/Character/WarriorClass.cpp
        Game/Character/WarriorClass.h

)

target_include_directories( ${PROJECT_NAME} PUBLIC ${IMGUI_PATH}  ${SDL2_MIXER_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC imgui glfw ${OPENGL_LIBRARIES} SDL2_mixer SDL2 ${SDL2_LIBRARIES} ) # windows 'glfw3', with a 3, linux 'glfw' no 3,
                                                                                            # but you still install libglfw3 with apt (debian family)
