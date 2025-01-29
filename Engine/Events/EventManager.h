//
// Created by hous on 12/29/24.
//

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include <vector>
#include <memory>
class Event {
public:
    virtual ~Event() = default;
    virtual void handle() = 0;
};
class EventManager {
    std::vector<std::unique_ptr<Event>> events;
public:
    void addEvent(std::unique_ptr<Event> event) {
        events.push_back(std::move(event));
    }

    void handleEvents() {
        for (auto& event : events) {
            event->handle();
        }
        events.clear();
    }
};

#endif //EVENTMANAGER_H
