//
// Created by hous on 12/19/24.
//

#ifndef GAME_H
#define GAME_H
#include <vector>

// #include "Playable.h"
#include "../../Game/UI/GameUI.h"
#include "../Renderer/Renderer.h"

enum GameState {PLAYING, UI_INTERRUPT};
class Game {
    public:
    //J'utilise des Pointeurs pour pouvoir delete
        GameUI* gameUI{};
        GameState state{};
        Renderer* renderer{};


    Game();
    virtual ~Game();// TODO :  JE SUIS FICHU

};

#endif //GAME_H
