//
// Created by hous on 12/29/24.
//

#ifndef USEORBUYITEMEVENT_H
#define USEORBUYITEMEVENT_H
#include "../../Engine/Events/EventManager.h"
#include "../Inventory/Item.h"

class UseOrBuyItemEvent:public Event{
    public:
    Item& item;
    explicit UseOrBuyItemEvent(Item& _item): item(_item) {
    } ;
    void handle() override {

    };
};
#endif //USEORBUYITEMEVENT_H
