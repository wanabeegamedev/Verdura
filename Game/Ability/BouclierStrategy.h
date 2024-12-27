//
// Created by hous on 12/25/24.
//

#ifndef BOUCLIERSTRATEGY_H
#define BOUCLIERSTRATEGY_H
#include "Attack.h"
#include "DefenseStrategy.h"



class BouclierStrategy:public DefenseStrategy
{
public:
     void doDefense(Attack* attack) override;
};



#endif //BOUCLIERSTRATEGY_H
