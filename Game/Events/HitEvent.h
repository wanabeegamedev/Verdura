//
// Created by hous on 12/29/24.
//

#ifndef HITEVENT_H
#define HITEVENT_H
#include "../../Engine/Events/EventManager.h"
#include "../../Engine/Sound/SoundManager.h"
#include "../Ability/Attack.h"
#include "../Character/Character.h"

class HitEvent:public Event {
public:
    Character& agresseur;
    Character& victime;
    Attack& attack;
    SoundManager& soundManager;
    HitEvent(Character& _agresseur,Character& _victime,
        Attack& _attack,SoundManager& _soundManager);
    void handle() override;
};

inline HitEvent::HitEvent(Character& _agresseur, Character& _victime,
    Attack &_attack, SoundManager& _soundManager):
    agresseur(_agresseur),
    victime(_victime),
    attack(_attack),
    soundManager(_soundManager) {
}

inline void HitEvent::handle()
{
    //agresseur
    //soundManager.playSound(attack.soundName);
}
#endif //HITEVENT_H
