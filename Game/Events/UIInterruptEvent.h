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
    std::string text;
    UIInterruptEvent(GameUI* _gameUI,
        SoundManager* _soundManager,const std::string& _text): gameUI(_gameUI),
    soundManager(_soundManager),text(_text){}

    ~UIInterruptEvent() override = default;
    void handle() override;
};

inline void UIInterruptEvent::handle() {
    gameUI->stateFlag = 1;//UI_INTERRUPT
    gameUI->toggle_info_once_window(text);
    soundManager->playSound("InterruptSound");

}
#endif //UIINTERRUPTEVENT_H
