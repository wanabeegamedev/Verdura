//
// Created by hous on 12/19/24.
//

#ifndef GAME_H
#define GAME_H
#include <vector>

// #include "Playable.h"
#include "../../Game/UI/GameUI.h"
#include "../Renderer/Renderer.h"

enum GameState {PLAYING, UI_INTERRUPT, OVER};
class Game {
    public:

        GameUI* gameUI{};
        GameState state{};
        Renderer* renderer{};


    Game(){};
    virtual ~Game()=default;

};

#endif //GAME_H
