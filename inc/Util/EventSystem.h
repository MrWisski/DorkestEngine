#pragma once
#include <map>
#include <list>
#include <typeindex>
#include <string>
#include <Util/Log.h>

/*
* //make an event class.
class keyEvent : public Event {
public:
    keyEvent(GLFWwindow* pWindow, int key, int scancode, int action, int mods) : pWindow(pWindow), key(key), scancode(scancode), action(action), mods(mods) {}
    ~keyEvent() {}

    GLFWwindow* pWindow;
    int key, scancode, action, mods;

};

//Call the publish event to push it out to subs
    bus->publish<keyEvent>(new keyEvent(pWindow, key, scancode, action, mods));

//Sub to event.

    init(){
        ...
        bus->subscribe(this, &<this class name>::onKeyEvent);
        ...
    }

    void onKeyEvent(keyEvent* e) {
        if (e->key == GLFW_KEY_R && e->action == GLFW_PRESS)
            debug("r pressed");
    }
*/


class Event
{
protected:
    virtual ~Event() {};
};

// This is the interface for MemberFunctionHandler that each specialization will use
class HandlerFunctionBase {
public:
    // Call the member function
    void exec(Event* evnt) {
        call(evnt);
    }
private:
    // Implemented by MemberFunctionHandler
    virtual void call(Event* evnt) = 0;
};

template<class T, class EventType>
class MemberFunctionHandler : public HandlerFunctionBase
{
public:
    typedef void (T::* MemberFunction)(EventType*);

    MemberFunctionHandler(T* instance, MemberFunction memberFunction) : instance{ instance }, memberFunction{ memberFunction } {};

    void call(Event* evnt) {
        // Cast event to the correct type and call member function
        (instance->*memberFunction)(static_cast<EventType*>(evnt));
    }
private:
    // Pointer to class instance
    T* instance;

    // Pointer to member function
    MemberFunction memberFunction;
};

typedef std::list<HandlerFunctionBase*> HandlerList;

class EventBus {
public:
    template<typename EventType>
    void publish(EventType* evnt) {
        HandlerList* handlers = subscribers[typeid(EventType)];

        if (handlers == nullptr) {
            return;
        }

        for (auto& handler : *handlers) {
            if (handler != nullptr) {
                handler->exec(evnt);
            }
        }
    }

    template<class T, class EventType>
    void subscribe(T* instance, void (T::* memberFunction)(EventType*)) {
        HandlerList* handlers = subscribers[typeid(EventType)];

        //First time initialization
        if (handlers == nullptr) {
            handlers = new HandlerList();
            subscribers[typeid(EventType)] = handlers;
        }

        handlers->push_back(new MemberFunctionHandler<T, EventType>(instance, memberFunction));
    }
private:
    std::map<std::type_index, HandlerList*> subscribers;
};


