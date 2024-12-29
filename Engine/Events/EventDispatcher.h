//
// Created by hous on 12/29/24.
//

#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H
#include <vector>
#include <memory>
class Event {
public:
    virtual ~Event() = default;
    virtual void handle() = 0;
};

class EventDispatcher {
    std::vector<std::unique_ptr<Event>> events;
public:
    void addEvent(std::unique_ptr<Event> event) {
        events.push_back(std::move(event));
    }
    void dispatch() {
        for (auto& event : events) {
            event->handle();
        }
        events.clear();
    }
};
#endif //EVENTDISPATCHER_H
