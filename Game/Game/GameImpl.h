//
// Created by hous on 12/28/24.
//

#ifndef GAMEIMPL_H
#define GAMEIMPL_H
#include "../../Engine/Common/Game.h"
#include "../../Engine/Events/EventManager.h"
#include "../Game/Events/HitEvent.h"
#include "../Game/Events/LevelingEvent.h"
#include "../Game/Events/GameEndEvent.h"
#include "../Game/Events/UIInterruptEvent.h"
#include "../../Engine/Sound/SoundManager.h"
#include "../Character/DamageManager.h"
#include "../Character/Enemy.h"
#include "../Character/Hero.h"
#include "../Character/Leveling.h"

class GameImpl:public Game {
    public:
        Hero* hero{};
        std::vector<OBJMeshFlyWeight> enemiesMeshes;
        std::vector<Enemy> enemies;
        DamageManager damageManager;
        SoundManager soundManager;
        Leveling leveling;// J'aurais dû l'appeler LevelingManager
        EventManager eventManager;
        void handleHeroInputs(float deltaTime);
        void updateHero(float deltaTime);
        void updateEnemies(float deltaTime);
        void renderGame(float);
        void runGame(float);
        GameImpl();
        void Initialize();
        ~GameImpl() override;

        OBJMesh protoMeshEnemy;
        OBJMesh protoMeshHero;

};



#endif //GAMEIMPL_H
