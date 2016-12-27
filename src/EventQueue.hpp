//
// Created by themegatb on 12/27/16.
//

#ifndef SMARTHOME_EVENTQUEUE_HPP
#define SMARTHOME_EVENTQUEUE_HPP

#include <vector>
#include <iostream>
#include "SafeQueue.hpp"

template<class T>
class Observer {
public:
    virtual void process(T event)= 0;
};

template<class T>
class EventQueue {
    SafeQueue<T> *input_queue;
    std::vector<Observer<T> *> observers;
public:
    EventQueue(SafeQueue<T> *queue) {
        this->input_queue = queue;
    };

    void addObserver(Observer<T> *o) {
        this->observers.push_back(o);
    };

    void observeOnce() {
        T event = this->input_queue->take();
        for (Observer<T> *observer : this->observers)
            observer->process(event);
    };
};

#endif //SMARTHOME_EVENTQUEUE_HPP
