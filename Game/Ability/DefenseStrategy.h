//
// Created by hous on 12/25/24.
//

#ifndef DEFENSESTRATEGY_H
#define DEFENSESTRATEGY_H
#include "Attack.h"


class DefenseStrategy {
public:
    virtual ~DefenseStrategy() = default;
    virtual void  doDefense(Attack* attack){};
    DefenseStrategy()= default;
};



#endif //DEFENSESTRATEGY_H
