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
        //std::vector<Playable> playables;
    //J'utilise des Pointeurs pour pouvoir delete
        GameUI* gameUI{};
        GameState state{};
        Renderer* renderer{};

    Game();
    virtual ~Game();

};

#endif //GAME_H
