//
// Created by hous on 12/25/24.
//

#include "Enemy.h"
void Enemy::attackSuccessful() override
{
}
void Enemy::attackReceived(float damage) override
{
   currentHp-=damage;
}
