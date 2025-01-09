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
#include <GLFW/glfw3.h>
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

// MY INCLUDES
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>
#include <vector>
#include <set>
#include <glm/glm.hpp>

#include <glm/matrix.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/Camera/Camera.h"
#include "Engine/Common/Game.h"
#include "Engine/Common/math_helper.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Mesh/MeshLoader.h"
#include "Engine/Mesh/OBJMesh.h"
#include "Engine/Mesh/OBJMeshFlyWeight.h"
#include "Engine/ParticleEffect/ParticleManager.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Shader/program.h"
#include "Engine/Shader/shader.h"
#include "Engine/Sound/SoundManager.h"
#include "Game/Ability/DamageReductionStrategy.h"
#include "Game/Character/DamageManager.h"
#include "Game/Character/Enemy.h"
#include "Game/Character/Hero.h"
#include "Game/Events/HitEvent.h"
#include "Game/Events/UIInterruptEvent.h"
#include "Game/UI/GameUI.h"


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


Camera camera;
double lastFrame = 0.0; // Time of the last frame // Belongs to RENDERER
double deltaTime = 0.0; // Time between current and last frame // Belongs to RENDERER


int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
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
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    Renderer renderer;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Verdura RPG", nullptr, nullptr);

    glfwMakeContextCurrent(window);

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

    //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);


    if(!gladLoadGL())
        {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return -1;
        }
    // Fin Initialization de glew &  glfw

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


    auto* program = new Program(vShader.shader_id(), fShader.shader_id());
    program->setName("t_pose_j1");

    mesh.addProgram(program);
    mesh.setCurrentProgram("t_pose_j1");
    //mesh2.scale(glm::vec3(1.f, 1.f, 1.f));
    mesh2.addProgram(program);
    mesh2.setCurrentProgram("t_pose_j1");

    SoundManager soundManager;
    soundManager.loadSound("gameStart", "/home/hous/CLionProjects/Verdura/Engine/Sound/gamestart-272829.mp3");
    soundManager.loadSound("fireSound", "/home/hous/CLionProjects/Verdura/Engine/Sound/8-bit-laser-151672.mp3");
    soundManager.loadSound("pain1", "/home/hous/CLionProjects/Verdura/Engine/Sound/088436_hurt-1-82785.mp3");
    soundManager.loadSound("music1", "/home/hous/CLionProjects/Verdura/Engine/Sound/8bit-music-for-game-68698.mp3");

    soundManager.playSequential("gameStart","music1");

    Shader vShaderFire("/home/hous/CLionProjects/Verdura/Game/Shaders/vertex_fire_ice_particle.txt",GL_VERTEX_SHADER);
    Shader fShaderFire("/home/hous/CLionProjects/Verdura/Game/Shaders/fragment_fire_ice_particle.txt",GL_FRAGMENT_SHADER);
    Program programFire(vShaderFire.shader_id(),fShaderFire.shader_id());
    programFire.setName("fireProgram");
    Particle particleFire("/home/hous/CLionProjects/Verdura/Engine/ParticleEffect/fireball.png",
                        mesh2.position,
                        mesh2.facingDirection,
                        &programFire);
    particleFire.load();
    ParticleManager particleManager{};
    particleManager.prepareObjectPool(particleFire);
    GameUI gameUI{};

    Stats stats;
    stats.setHP(250.f);
    Hero hero1(&mesh2,stats,"Le Héros");

    std::vector<OBJMeshFlyWeight> enemiesMeshes;
    std::vector<Enemy> enemies;
    enemiesMeshes.reserve(24);
    enemies.reserve(24);
    Attack* attack = new Attack(
            glm::vec3(0.0f),
            0,
            "/home/hous/CLionProjects/Verdura/Engine/ParticleEffect/fireball.png",
            10.0f,
            &programFire
        );
    float fireDelayBase = 3.0f;

    std::vector<glm::vec3> enemyPositions = generateEnemyPositions(0.0f, 100.0f, -8.0f, 8.0f, 40, 20);
    for (int i = 0; i < 20; i++) {
        OBJMeshFlyWeight _mesh(enemyPositions[i]);
        enemiesMeshes.push_back(_mesh);

        //std::cout << "Position: (" << _mesh.position.x << ", " << _mesh.position.y << ", " << _mesh.position.z << ")\n";
        Enemy enemy(&mesh,&enemiesMeshes.back());
        DefenseStrategy* defenseStrategy = new DefenseStrategy();
        WarriorClass* warriorClass = new WarriorClass(attack, defenseStrategy, fireDelayBase);
        warriorClass->setName(WIZARD_CLASS_NAME);
        enemy.setClass(warriorClass);
        enemies.push_back(enemy);
    }


    //TODO
    Attack* heroAttack = new Attack(
            glm::vec3(0.0f),
            0,
            "/home/hous/CLionProjects/Verdura/Engine/ParticleEffect/sword.png",
            10.0f,
            &programFire
        );

    for (Particle& particle : heroAttack->particleManager.particlesObjectPool) {
        particle.velocity  = 7.0f;
    }
    float heroDelayBase = .0f;
    DefenseStrategy* heroDefenseStrategy = new DamageReductionStrategy();
    WarriorClass heroClassSword(heroAttack,heroDefenseStrategy,heroDelayBase);
    heroClassSword.setName(KNIGHT_CLASS_NAME);
    gameUI.add_class_to_track(&heroClassSword);
    gameUI.setStats(&stats);




    DamageManager damageManager;
    EventManager eventManager;

    eventManager.addEvent(std::make_unique<UIInterruptEvent>(&gameUI,&soundManager,
        "Vous êtes un chevalier envoyé par la couronne pour vaincre la guilde des sorciers rebelles!\n"
        "En ces temps de guerres, ils peuvent envoyer un seul homme, mais vous êtes spécial\n"
        "En tant que résultat d'une expérience magique menée sur vous, vous pouvez apprendre des enseignements\n"
        "magiques et vous soignez avec des potions quand vous abbatez plusieurs enemy.\n"
        "ZSQD : Caméra (cela vous aidera à lire ce texte, mais pas que).\n"
        "Flèches directionnelles: Bougez .\n"
        "L : Attaque de classe 1, chevalier .\n"
        "M : Attaque de classe 2, si vous debloquez la classe sorcier (pas rebelle!)\n"
        "On va bien s'amuser, c\'est parti !"));
    //std::cout << particleManager.particlesObjectPool[0].velocity << std::endl;
    //std::cout << attack->particleManager.particlesObjectPool[0].velocity << std::endl;

    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1); // V-SYNC
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef __EMSCRIPTEN__
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        glfwPollEvents();

        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        gameUI.handleInputs(deltaTime);
        ImGui::Render();


        //Le Renderer GLFW Intervient
        int display_w=1280, display_h=720;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
            clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // If GameState == UI_INTERRUPT // Render but not update
        // If GameState == PLAYING // Update && Render
        // // If GameState == OVER // Render but not update

        renderer.updateCamera(deltaTime);

    if (gameUI.stateFlag == GameState::PLAYING)//TODO
    {

        //hero1.handleInputs();
        if (ImGui::IsKeyPressed(ImGuiKey_M,false))
        {
            heroAttack->particleManager.releaseFromObjectPool(hero1.characterMesh->position,
                hero1.characterMesh->facingDirection,soundManager);
        }
        hero1.characterMesh->handleInputs(deltaTime); //TODO should use a specific InputHandler class to include imgui jsut once
        // TODO dans Enemy.update
        // TODO Travail de la class DamageManager; qui va le faire sur Attack
        // puis lancer le HitEvent avec le soundManager et la position de la
        // particule

        //TODO hero
        heroAttack->particleManager.update(deltaTime);
        for (Particle& particle : heroAttack->particleManager.particlesObjectPool) {
            for (Enemy& enemy : enemies)
                if (particle.isActive) {
                    if (damageManager.checkCollision(particle.position, enemy.flyweightMesh->position)) {
                        particle.isActive = false;
                        eventManager.addEvent(std::make_unique<HitEvent>(hero1, enemy, soundManager));
                    }
                }
        }


        //updateEnemies(hero,deltaTime,soundManager)
       for (Enemy& enemy : enemies) {
            //TODO Enemy.update();
            enemy.alignToHero(hero1.characterMesh->position);
            if (enemy.isCloseEnough(hero1.characterMesh->position))
                 enemy.doAttack(deltaTime, soundManager);
            if (enemy.warriorClass && enemy.warriorClass->attack) {
                //enemy.warriorClass->attack->particleManager.update(deltaTime);
                for (Particle& particle : enemy.warriorClass->attack->particleManager.particlesObjectPool) {
                    if (particle.isActive) {
                        if (damageManager.checkCollision(particle.position, hero1.characterMesh->position)) {
                            particle.isActive = false;
                            soundManager.playSound("pain1");
                            eventManager.addEvent(std::make_unique<HitEvent>(enemy, hero1, soundManager));
                        }
                    }
                }
            }
        }

        if (enemies.size() > 0)
        enemies.back().warriorClass->attack->particleManager.update(deltaTime);

        eventManager.handleEvents();


    }
        //PARTIE RENDER
        renderer.renderParticles(heroAttack->particleManager);
        renderer.renderMeshOBJ(*hero1.characterMesh,deltaTime);
        if (enemies.size() > 0)
        renderer.renderParticles(enemies.back().warriorClass->attack->particleManager);
       
        for (Enemy& enemy : enemies) {
            renderer.renderMeshOBJFromFlyWeight(mesh, *enemy.flyweightMesh, deltaTime);
        }

        glBindVertexArray(0);
        glfwSwapBuffers(window);
        lastFrame = currentFrame;
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