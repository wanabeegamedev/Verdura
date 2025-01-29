//
// Created by hous on 12/25/24.
//

#ifndef DAMAGEREDUCTIONSTRATEGY_H
#define DAMAGEREDUCTIONSTRATEGY_H
#include "Attack.h"
#include "DefenseStrategy.h"



class DamageReductionStrategy:public DefenseStrategy
{
public:
     float doDefense(float _damage) override {
          return _damage*0.80f;
     };
};



#endif //DAMAGEREDUCTIONSTRATEGY_H
