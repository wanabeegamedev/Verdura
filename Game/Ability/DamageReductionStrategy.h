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
     void doDefense(Attack* attack) override{};
};



#endif //DAMAGEREDUCTIONSTRATEGY_H
