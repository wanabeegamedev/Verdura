//
// Created by hous on 12/28/24.
//

#ifndef GAMEIMPL_H
#define GAMEIMPL_H
#include "../../Engine/Common/Game.h"
#include "../../Engine/Events/EventManager.h"
#include "../../Engine/Sound/SoundManager.h"
#include "../Character/DamageManager.h"
#include "../Character/Enemy.h"
#include "../Character/Hero.h"
#include "../Character/Leveling.h"
#include "../Inventory/Shop.h"


class GameImpl:public Game {
    public:
        // Cette implementation utilise un seul hero; mais GameImpl est codé pour reussir à manager plusieurs
        std::vector<Hero> heros;
        std::vector<Enemy> enemies;
        DamageManager damageManager;
        SoundManager soundManager;
        Leveling levelingManager;// J'aurais dû l'appeler LevelingManager
        EventManager eventManager;
        Shop shop;

        //TODO 2 UI Stats
        //TODO 2 Warrior Classes
        //TODO 3 UI update
        //TODO 4 LevelUp
        //TODO 6 UI Image Inventory
        //TODO 5 Shop
        //TODO 7 Sounds
        void setRenderer(Renderer&);
        void setGameUI(GameUI&);

        void update();
        void addHero(Hero& hero);
        void addEnemy(Enemy& enemy);
        void renderGame();
        void computeDamage();// DAMAGE MANAGER DOES THAT
        void updateGame();
        void runGame();

        GameImpl();
        void Initialize();
        ~GameImpl() override;

};



#endif //GAMEIMPL_H
