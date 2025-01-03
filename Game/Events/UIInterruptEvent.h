//
// Created by hous on 12/30/24.
//

#ifndef UIINTERRUPTEVENT_H
#define UIINTERRUPTEVENT_H
#include "../../Engine/Events/EventManager.h"
#include "../UI/GameUI.h"
// const std::string interruptSound = "InterruptSound";
class UIInterruptEvent:public Event {
    public:
    GameUI* gameUI;
    SoundManager* soundManager{};
    explicit UIInterruptEvent(GameUI* _gameUI,
        SoundManager* _soundManager): gameUI(_gameUI),
    soundManager(_soundManager){}

    ~UIInterruptEvent() override = default;
    void handle() override {
        gameUI->stateFlag = 1;//UI_INTERRUPT
        gameUI->toggle_info_once_window("Ceci est une info");
        soundManager->playSound("InterruptSound");
  }
};
#endif //UIINTERRUPTEVENT_H
