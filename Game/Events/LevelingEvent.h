//
// Created by hous on 1/9/25.
//

#ifndef LEVELINGEVENT_H
#define LEVELINGEVENT_H
#include "../../Engine/Events/EventManager.h"
#include "../../Engine/Sound/SoundManager.h"
#include "../Character/Hero.h"
#include "../UI/GameUI.h"

class LevelingEvent:public Event {
public:
    Hero* hero;
    GameUI* gameUI;
    SoundManager* soundManager{};
    Leveling* leveling;
    int XP;

    LevelingEvent(Hero* _hero,GameUI* _gameUI,SoundManager* _soundManager,Leveling* _leveling,int _XP):
        hero(_hero), gameUI(_gameUI),
        soundManager(_soundManager),
        leveling(_leveling),
    XP(_XP){}
    void handle() override;
};

inline void LevelingEvent::handle() {

    if (leveling->checkXP(XP)) {

        gameUI->stateFlag = 1;
        gameUI->toggle_info_reward_window();
        soundManager->playSound("winsquare");
    }

}

#endif //LEVELINGEVENT_H
