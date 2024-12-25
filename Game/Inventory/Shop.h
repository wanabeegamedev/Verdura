//
// Created by hous on 12/25/24.
//

#ifndef SHOP_H
#define SHOP_H
#include <vector>

#include "Item.h"
#include "../Character/Character.h"


class Shop {
public:
    std::vector<Item> items;
    void addItem(Item item);
    void sellItem(Item item,Character& character);
};



#endif //SHOP_H
