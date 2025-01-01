#include <glad/glad.h>
// IMGUI INCLUDES
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui//backends/imgui_impl_opengl3.h"
#include <cstdio>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

// MY INCLUDES
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <set>
#include <glm/glm.hpp>

#include <glm/matrix.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/Camera/Camera.h"
#include "Engine/Mesh/MeshLoader.h"
#include "Engine/Mesh/OBJMesh.h"
#include "Engine/ParticleEffect/ParticleManager.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Shader/program.h"
#include "Engine/Shader/shader.h"
#include "Engine/Sound/SoundManager.h"
#include "Game/Character/DamageManager.h"
#include "Game/Character/Hero.h"
#include "Game/UI/GameUI.h"


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Map dimensions
const float mapWidth = 2000.0f, mapHeight = 720.0f;


Camera camera; // Belongs to RENDERER
// Global variables for timing (RENDERER)
double lastFrame = 0.0; // Time of the last frame // Belongs to RENDERER
double deltaTime = 0.0; // Time between current and last frame // Belongs to RENDERER


int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    // PARTIE IMGUI
    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    Renderer renderer;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    //glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSwapInterval(1);

    //  Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);


    if(!gladLoadGL())
        {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return -1;
        }
    // After Initialization of glew and glfw

    MeshLoader loader;

    OBJMesh mesh("/home/hous/CLionProjects/Verdura/Game/Assets/Characters/Mage/Mage.obj");
    mesh.set_current_texture_path("/home/hous/CLionProjects/Verdura/Game/Assets/Characters/Mage/mage_texture.png");
    mesh.set_position(glm::vec3(-8,.0f,.0f));//TODO utiliser à la création de la mesh
    loader.LoadObjMesh(mesh);

    OBJMesh mesh2("/home/hous/CLionProjects/Verdura/Game/Assets/Characters/Knight/Knight.obj");
    mesh2.set_current_texture_path("/home/hous/CLionProjects/Verdura/Game/Assets/Characters/Knight/texture_1.png");
    mesh2.set_position(glm::vec3(0,.0f,.0f));
    loader.LoadObjMesh(mesh2);


    Shader vShader("/home/hous/CLionProjects/Verdura/Game/Shaders/vertex_character.txt",GL_VERTEX_SHADER);
    Shader fShader("/home/hous/CLionProjects/Verdura/Game/Shaders/fragment_character.txt",GL_FRAGMENT_SHADER);
    Program program(vShader.shader_id(),fShader.shader_id());
    program.setName("t_pose_j1");


    mesh.addProgram(&program);
    mesh.setCurrentProgram("t_pose_j1");
    mesh2.scale(glm::vec3(1.f, 1.f, 1.f)); // Scaler

    mesh2.addProgram(&program);
    mesh2.setCurrentProgram("t_pose_j1");


    /*TODO Move to checkProgram, all values in uniformCache must be Positive
     *if (modelLoc == -1 || viewLoc == -1 || projectionLoc == -1 || textureLoc == -1) {
        std::cerr << "One or more uniforms not found in the shader program!" << std::endl;
    }*/


    SoundManager soundManager;
    soundManager.loadSound("gameStart", "/home/hous/CLionProjects/Verdura/Engine/Sound/gamestart-272829.mp3");
    //soundManager.playSound("gameStart");
    soundManager.loadSound("fireSound", "/home/hous/CLionProjects/Verdura/Engine/Sound/8-bit-laser-151672.mp3");
    soundManager.loadSound("pain1", "/home/hous/CLionProjects/Verdura/Engine/Sound/088436_hurt-1-82785.mp3");
    soundManager.loadSound("music1", "/home/hous/CLionProjects/Verdura/Engine/Sound/8bit-music-for-game-68698.mp3");
    //soundManager.playSound("music1");

    soundManager.playSequential("gameStart","music1");
    Shader vShaderFire("/home/hous/CLionProjects/Verdura/Game/Shaders/vertex_fire_ice_particle.txt",GL_VERTEX_SHADER);
    Shader fShaderFire("/home/hous/CLionProjects/Verdura/Game/Shaders/fragment_fire_ice_particle.txt",GL_FRAGMENT_SHADER);
    Program programFire(vShaderFire.shader_id(),fShaderFire.shader_id());
    programFire.setName("fireProgram");
    Particle particleFire("/home/hous/CLionProjects/Verdura/Engine/ParticleEffect/fireball.png",
                        0.3f,
                        mesh2.position,
                        mesh2.facingDirection,
                        &programFire);
    //particleFire.isActive = true;
    //particleFire.scale(glm::vec3(0.07f));
    particleFire.load();
    ParticleManager particleManager{};
    particleManager.prepareObjectPool(particleFire);
    GameUI gameUI{};

    Stats stats;
    Inventory inventory;
    Hero hero1(mesh2,stats,inventory,"Le Héros");
    DamageManager damageManager;

    glEnable(GL_DEPTH_TEST); //  depth testing
    glfwSwapInterval(1); // V-SYNC
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        glfwPollEvents();
//      GameUI.Initialize();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // END GameUI.Initialize();
/*
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
*/
        //GameUI.handleInputs(); // va hériter de UI qui manage des GameObjects et va elle, manager des Objets concrets;
        /*if (show_inventory)
        {
            ImGui::Begin("Ma sacoche de héros", &show_inventory);
            ImGui::Text("Qu\'est-ce qu\'on a de beau ?'");
            if (ImGui::Button("J\'ai fini !"))
                show_inventory = false;
            ImGui::End();
        }*/
        gameUI.handleInputs();
        ImGui::Render();


        //TODO Rendering; ICI Le Renderer GLFW Intervient

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // opengl render ici, when DearImGUI is done

        // RENDERER TAKE OVER
        lastFrame = glfwGetTime();
        deltaTime = glfwGetTime() - lastFrame;
        
        hero1.characterMesh.handleInputs(deltaTime); //TODO should use a specific InputHandler class to include imgui jsut once
        //mesh2.handleInputs(camera.camera_front(),camera.camera_right(),deltaTime); //TODO should use a specific InputHandler class to include imgui jsut once

        camera.handleInputs(deltaTime);


        camera.updateCameraVectors();
        camera.update();

        // If GameState == UI_INTERRUPT // Render but not update
        //mesh.Render(camera,deltaTime);
        //mesh2.Render(camera,deltaTime);
        renderer.renderMeshOBJ(mesh,deltaTime);
        renderer.renderMeshOBJ(hero1.characterMesh,deltaTime);


        if (ImGui::IsKeyPressed(ImGuiKey_M,false))
        {
            particleManager.releaseFromObjectPool(hero1.characterMesh.position,hero1.characterMesh.facingDirection);
            soundManager.playSound("fireSound");//TODO Travail de la classe Attack, Qui a besoin d'un soundManager
            //std::cout << "Position: (" << mesh2.position.x << ", " << mesh2.position.y << ", " << mesh2.position.z << ")\n";
        }

        //particleFire.update(deltaTime);
        //particleFire.renderParticle(&programFire,deltaTime);

        //TODO Travail de la class DamageManager; qui va le faire sur Attack
        // puis lancer le HitEvent avec le soundManager et la position de la
        // particule


        particleManager.update(deltaTime);
        //Sans la ref c'est une copie
        for (Particle& particle :particleManager.particlesObjectPool)
            if (particle.isActive)
                if (damageManager.checkCollision(particle.position,mesh.position))
                {
                    particle.isActive = false;
                    soundManager.playSound("pain1");
                }
        renderer.renderParticles(particleManager);


        glBindVertexArray(0);
        glfwSwapBuffers(window);
        //TODO ICI Le Renderer GLFW a terminé
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window); // Renderer.window, //RENDERER class attributes
    glfwTerminate();

    return 0;
}