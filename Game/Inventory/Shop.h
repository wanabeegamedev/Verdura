//
// Created by hous on 12/25/24.
//

#ifndef SHOP_H
#define SHOP_H
#include <vector>

#include "Inventory.h"
#include "Item.h"
#include "../Character/Character.h"


class Shop: public Inventory {
public:
    Shop();
    ~Shop();
    void sellItem(Item item,Character& character);
};



#endif //SHOP_H
