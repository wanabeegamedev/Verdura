//
// Created by hous on 12/25/24.
//

#ifndef ITEM_H
#define ITEM_H
#include <string>


class Item {
public:
    std::string itemName;
    std::string itemDescription;
    int itemCost{};
    Item(std::string itemName, std::string itemDescription, int itemCost);
    Item(){};
};



#endif //ITEM_H
