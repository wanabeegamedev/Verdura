//
// Created by hous on 12/28/24.
//

#include "GameImpl.h"

#include <algorithm>

#include "../../Engine/Common/math_helper.h"
#include "../../Engine/Mesh/MeshLoader.h"
#include "../../Engine/Shader/shader.h"
#include "../Ability/DamageReductionStrategy.h"



void GameImpl::handleHeroInputs(float deltaTime) {
    hero->handleInputs(deltaTime,soundManager);
}
void GameImpl::updateHero(float deltaTime) {

    for (WarriorClass*& wc : hero->HeroClasses )
        wc->attack->particleManager.update(deltaTime);
    for (WarriorClass*& wc : hero->HeroClasses )
        for (Particle& particle : wc->attack->particleManager.particlesObjectPool) {
            for (Enemy& enemy : enemies)
                if (particle.isActive) {
                    if (damageManager.checkCollision(particle.position, enemy.flyweightMesh->position)) {
                        particle.isActive = false;
                        eventManager.addEvent(std::make_unique<HitEvent>(*hero, enemy, soundManager));
                        eventManager.addEvent(std::make_unique<LevelingEvent>(hero,gameUI,&soundManager,&leveling,hero->stats->currentExp));

                    }
                }
        }
}
void GameImpl::updateEnemies(float deltaTime) {
    if (enemies.size() > 0)
        for (Enemy& enemy : enemies) {
            enemy.alignToHero(hero->characterMesh->position);
            if (enemy.isCloseEnough(hero->characterMesh->position))
                enemy.doAttack(deltaTime, soundManager);
            if (enemy.warriorClass && enemy.warriorClass->attack) {
                for (Particle& particle : enemy.warriorClass->attack->particleManager.particlesObjectPool) {
                    if (particle.isActive) {
                        if (damageManager.checkCollision(particle.position, hero->characterMesh->position)) {
                            particle.isActive = false;
                            soundManager.playSound("pain1");
                            eventManager.addEvent(std::make_unique<HitEvent>(enemy, *hero, soundManager));
                        }
                    }
                }
            }
        }
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& enemy) { return enemy.toRemove; }), enemies.end());
    if (enemies.size() > 0)
        enemies.back().warriorClass->attack->particleManager.update(deltaTime);
   /* else
        eventManager.addEvent(std::make_unique<GameEndEvent>(true, soundManager));*/

}
void GameImpl::renderGame(float deltaTime) {
    for (WarriorClass*& wc : hero->HeroClasses )
        renderer->renderParticles(wc->attack->particleManager);
    renderer->renderMeshOBJ(*hero->characterMesh,deltaTime);
    if (enemies.size() > 0)
        renderer->renderParticles(enemies.back().warriorClass->attack->particleManager);
    for (Enemy& enemy : enemies) {
        renderer->renderMeshOBJFromFlyWeight(protoMeshEnemy, *enemy.flyweightMesh, deltaTime);
    }
}

void GameImpl::runGame(float deltaTime) {
    handleHeroInputs(deltaTime);
    updateHero(deltaTime);
    updateEnemies(deltaTime);
    eventManager.handleEvents();
}

GameImpl::GameImpl(): protoMeshEnemy("/home/hous/CLionProjects/Verdura/Game/Assets/Characters/Mage/Mage.obj"),
protoMeshHero("/home/hous/CLionProjects/Verdura/Game/Assets/Characters/Knight/Knight.obj") {
    MeshLoader loader;

    //protoMeshEnemy = OBJMesh("/home/hous/CLionProjects/Verdura/Game/Assets/Characters/Mage/Mage.obj");
    protoMeshEnemy.set_current_texture_path(
        "/home/hous/CLionProjects/Verdura/Game/Assets/Characters/Mage/mage_texture.png");
    protoMeshEnemy.set_position(glm::vec3(-8, .0f, .0f)); //TODO utiliser à la création de la mesh
    loader.LoadObjMesh(protoMeshEnemy);

    //protoMeshHero = OBJMesh("/home/hous/CLionProjects/Verdura/Game/Assets/Characters/Knight/Knight.obj");
    protoMeshHero.set_current_texture_path(
        "/home/hous/CLionProjects/Verdura/Game/Assets/Characters/Knight/texture_1.png");
    protoMeshHero.set_position(glm::vec3(0, .0f, .0f));
    loader.LoadObjMesh(protoMeshHero);
}

void GameImpl::Initialize() {


    renderer = new Renderer();
    Shader vShader("/home/hous/CLionProjects/Verdura/Game/Shaders/vertex_character.txt",GL_VERTEX_SHADER);
    Shader fShader("/home/hous/CLionProjects/Verdura/Game/Shaders/fragment_character.txt",GL_FRAGMENT_SHADER);


    auto* program = new Program(vShader.shader_id(), fShader.shader_id());
    program->setName("t_pose_j1");

    protoMeshEnemy.addProgram(program);
    protoMeshEnemy.setCurrentProgram("t_pose_j1");
    protoMeshHero.addProgram(program);
    protoMeshHero.setCurrentProgram("t_pose_j1");

    soundManager.loadSound("gameStart", "/home/hous/CLionProjects/Verdura/Engine/Sound/gamestart-272829.mp3");
    soundManager.loadSound("fireSound", "/home/hous/CLionProjects/Verdura/Engine/Sound/8-bit-laser-151672.mp3");
    soundManager.loadSound("pain1", "/home/hous/CLionProjects/Verdura/Engine/Sound/088436_hurt-1-82785.mp3");
    soundManager.loadSound("music1", "/home/hous/CLionProjects/Verdura/Engine/Sound/8bit-music-for-game-68698.mp3");
    // soundManager.loadSound("interruptSound", "/home/hous/CLionProjects/Verdura/Engine/Sound/8bit-music-for-game-68698.mp3");
    // soundManager.loadSound("winsquare", "/home/hous/CLionProjects/Verdura/Engine/Sound/8bit-music-for-game-68698.mp3");

    soundManager.playSequential("gameStart","music1");

    Shader vShaderFire("/home/hous/CLionProjects/Verdura/Game/Shaders/vertex_fire_ice_particle.txt",GL_VERTEX_SHADER);
    Shader fShaderFire("/home/hous/CLionProjects/Verdura/Game/Shaders/fragment_fire_ice_particle.txt",GL_FRAGMENT_SHADER);
    Program* programFire = new Program(vShaderFire.shader_id(),fShaderFire.shader_id());
    programFire->setName("fireProgram");
    Particle particleFire("/home/hous/CLionProjects/Verdura/Engine/ParticleEffect/fireball.png",
                        protoMeshHero.position,
                        protoMeshHero.facingDirection,
                        programFire);
    particleFire.load();
    ParticleManager particleManager{};
    particleManager.prepareObjectPool(particleFire);

    hero = new Hero(&protoMeshHero,"Le Héros");

    enemiesMeshes.reserve(24);
    enemies.reserve(24);
    Attack* attack = new Attack(
            glm::vec3(0.0f),
            0,
            "/home/hous/CLionProjects/Verdura/Engine/ParticleEffect/fireball.png",
            10.0f,
            programFire
        );
    float fireDelayBase = 3.0f;
    std::vector<glm::vec3> enemyPositions = generateEnemyPositions(0.0f, 100.0f, -8.0f, 8.0f, 40, 20);
    for (int i = 0; i < 20; i++) {
        OBJMeshFlyWeight _mesh(enemyPositions[i]);
        enemiesMeshes.push_back(_mesh);
        Enemy enemy(&protoMeshEnemy,&enemiesMeshes.back());
        DefenseStrategy* defenseStrategy = new DefenseStrategy();
        WarriorClass* warriorClass = new WarriorClass(attack, defenseStrategy, fireDelayBase);
        warriorClass->setName(WIZARD_CLASS_NAME);
        enemy.setClass(warriorClass);
        enemies.push_back(enemy);
    }

    Attack* heroAttack = new Attack(
            glm::vec3(0.0f),
            0,
            "/home/hous/CLionProjects/Verdura/Engine/ParticleEffect/sword.png",
            15.0f,
            programFire
        );
    Attack* heroAttackWizard = new Attack(
            glm::vec3(0.0f),
            0,
            "/home/hous/CLionProjects/Verdura/Engine/ParticleEffect/fireball.png",
            20.0f,//TODO use damage points in attack received
            programFire
        );
    for (Particle& particle : heroAttack->particleManager.particlesObjectPool) {
        particle.velocity  = 7.0f;
    }
    for (Particle& particle : heroAttackWizard->particleManager.particlesObjectPool) {
        particle.velocity  = 9.0f;
    }
    float heroDelayBase = .0f;
    DefenseStrategy* heroDefenseStrategy = new DamageReductionStrategy();
    WarriorClass* heroClassSword = new WarriorClass(heroAttack,heroDefenseStrategy,heroDelayBase);
    heroClassSword->setName(KNIGHT_CLASS_NAME);
    hero->HeroClasses.push_back(heroClassSword);

    WarriorClass* heroClassFire = new WarriorClass(heroAttackWizard,heroDefenseStrategy,heroDelayBase);
    heroClassFire->setName(WIZARD_CLASS_NAME);
    gameUI = new GameUI();
    gameUI->rewardClass = heroClassFire;
    gameUI->add_class_to_track(heroClassSword);
    gameUI->setHero(hero);
    gameUI->setStats(hero->stats);
    eventManager.addEvent(std::make_unique<UIInterruptEvent>(gameUI,&soundManager,
        "Vous êtes un chevalier envoyé par la couronne pour vaincre la guilde des sorciers rebelles!\n"
        "En ces temps de guerres, ils peuvent envoyer un seul homme, mais vous êtes spécial\n"
        "En tant que résultat d'une expérience magique menée sur vous, vous pouvez apprendre des enseignements\n"
        "magiques et vous soignez avec des potions quand vous abbatez plusieurs enemy.\n"
        "ZSQD : Caméra (cela vous aidera à lire ce texte, mais pas que).\n"
        "Flèches directionnelles: Bougez .\n"
        "L : Attaque de classe 1, chevalier .\n"
        "M : Attaque de classe 2, si vous debloquez la classe sorcier (pas rebelle!)\n"
        "On va bien s'amuser, c\'est parti !"));
}

GameImpl::~GameImpl() {
    // il y aura suppression auto des pointeurs intelligents partout
    delete gameUI;
    delete renderer;
    delete hero;
}
