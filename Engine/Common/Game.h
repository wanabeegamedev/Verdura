//
// Created by hous on 12/19/24.
//

#ifndef GAME_H
#define GAME_H
#include <vector>

// #include "Playable.h"
#include "../../Game/UI/GameUI.h"
#include "../Renderer/Renderer.h"

class Game {
    public:
        //std::vector<Playable> playables;
        GameUI gameUI;
        Renderer renderer;

    Game();
    ~Game();

};

#endif //GAME_H
