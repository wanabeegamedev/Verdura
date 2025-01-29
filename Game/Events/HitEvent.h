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
    // Attack& attack;
    SoundManager& soundManager;
    HitEvent(Character& _agresseur,Character& _victime,
       SoundManager& _soundManager,float _damage);
    float damage;
    void handle() override;
};

inline HitEvent::HitEvent(Character& _agresseur, Character& _victime,
    SoundManager& _soundManager, float _damage):
    agresseur(_agresseur),
    victime(_victime),
    soundManager(_soundManager),
    damage(_damage){
}

inline void HitEvent::handle()
{
    victime.attackReceived(damage);
    agresseur.attackSuccessful();
}
#endif //HITEVENT_H
