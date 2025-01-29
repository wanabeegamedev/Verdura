//
// Created by hous on 1/11/25.
//

#ifndef GAMEENDEVENT_H
#define GAMEENDEVENT_H
#include "../../Engine/Sound/SoundManager.h"
class GameEndEvent : public Event 
{
    public:
        GameUI* gameUI;
        SoundManager& soundManager;
        bool success{};
        GameEndEvent(bool _success, GameUI* _gameUI, SoundManager& _soundManager):
        soundManager(_soundManager),
        gameUI (_gameUI),
        success(_success){}
        virtual void handle() override;
};
inline void GameEndEvent::handle()
{
    gameUI->stateFlag = 2;//OVER
   if (success)
   {
       gameUI->toggle_info_once_window("Merci d'avoir joue jusqu'au bout !");
       soundManager.stopAll();
       soundManager.playSound("VictorySound");
   }
   else
   {
       gameUI->toggle_info_once_window("Merci d'avoir joue ! Vous pouvez toujours rejouer"
                                       "et gagner car vous etes devenu meilleur .");
       soundManager.playSound("DefeatSound");
   }
}
#endif //GAMEENDEVENT_H
