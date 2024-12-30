//
// Created by hous on 12/25/24.
//

#ifndef INVENTORY_H
#define INVENTORY_H
#include <vector>
#include <map>

#include "Item.h"


class Inventory {
public:
    std::map<Item,int> Items;
    void addItem(Item item);

};



#endif //INVENTORY_H
