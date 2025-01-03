//
// Created by hous on 12/25/24.
//

#include "Enemy.h"
#include <cassert>
Enemy::Enemy(OBJMesh* _characterMesh) {
   characterMesh = _characterMesh;
}

void Enemy::setClass(WarriorClass* _class)
{warriorClass=_class;}
void Enemy::attackSuccessful(){}
void Enemy::attackReceived(float damage)
{
   //assert(attack!=nullptr && "Enemy attack forgotten !");
   //TODO Deplacer dans make attack
   /*if (warriorClass == nullptr)
      throw std::runtime_error("Enemy attack forgotten!");*/
   currentHp-=damage;
}

void Enemy::alignToHero(const glm::vec3& heroPosition) {

   glm::vec3 direction = heroPosition - characterMesh->position;
   //std::cout<<"(" << direction.x << ", "<< direction.y << ", "<< direction.z << ")"<<std::endl;
   float absX = std::fabs(direction.x);
   float absZ = std::fabs(direction.z);
   glm::vec3 movementDirection(0.0f);
   int newFacingDirection = -1;

   // s'aligne au heros sur le axis le plus facile
   if (absX > absZ) {
      //  X-axis
      movementDirection.x = (direction.x > 0.0f) ? 1.0f : -1.0f;
      newFacingDirection = (movementDirection.x > 0.0f) ? 1 : 3;
   } else {
      //  Z-axis
      movementDirection.z = (direction.z > 0.0f) ? 1.0f : -1.0f;
      newFacingDirection = (movementDirection.z > 0.0f) ? 0 : 2;
   }
   //movementDirection = glm::normalize(movementDirection);

   float speed = 300000.f;
   /*characterMesh.position += movementDirection * speed*deltatime;*/

   characterMesh->model = glm::mat4(1.0f);
   characterMesh->model = glm::translate(characterMesh->model, characterMesh->position);
   characterMesh->rotate(glm::radians(90.0f * static_cast<float>(newFacingDirection)), movementRotate);

   characterMesh->facingDirection = newFacingDirection;
}
void Enemy::doAttack(float deltatime,SoundManager& soundManager) {
   warriorClass->doAttack(deltatime,characterMesh->position,
      characterMesh->facingDirection,soundManager);
}
Enemy::~Enemy()= default;