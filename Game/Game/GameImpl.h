//
// Created by hous on 12/28/24.
//

#ifndef GAMEIMPL_H
#define GAMEIMPL_H
#include "../../Engine/Common/Game.h"
#include "../Character/DamageManager.h"
#include "../Character/Enemy.h"
#include "../Character/Leveling.h"
#include "../Inventory/Shop.h"


class GameImpl:public Game {
    public:
        // Cette implementation utilise un seul hero; mais GameImpl est codé pour reussir à manager plusieurs

        //std::vector<Hero> heros;
        std::vector<Enemy> enemies;
        DamageManager damageManager;
        Leveling leveling;// J'aurais dû l'appeler LevelingManager
        Shop shop;

        void setRenderer(Renderer&);
        void setGameUI(GameUI&);

        void update();
        GameImpl();
        ~GameImpl() override;
};



#endif //GAMEIMPL_H
