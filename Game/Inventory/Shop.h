//
// Created by hous on 12/25/24.
//

#ifndef SHOP_H
#define SHOP_H
#include <vector>

#include "Inventory.h"
#include "Item.h"
#include "../Character/Hero.h"


class Shop: public Inventory {
public:
    Shop();
    ~Shop();
    void sellItem(Item item,Hero& hero);
    void buyItem(Item item,Hero& hero);
};



#endif //SHOP_H
