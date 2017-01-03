//
// Created by themegatb on 12/27/16.
//

#ifndef SMARTHOME_EVENTQUEUE_HPP
#define SMARTHOME_EVENTQUEUE_HPP

#include <vector>
#include <chrono>
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
    EventQueue(SafeQueue<T> *queue) : input_queue(queue) {};

    void addObserver(Observer<T> *o) {
        this->observers.push_back(o);
    };

    int observeOnce(std::chrono::duration<int, std::milli> timeout) {
        T event = this->input_queue->take(timeout);
        if (!event) return -1;
        for (Observer<T> *observer : this->observers)
            observer->process(event);
        return 1;
    };
};

#endif //SMARTHOME_EVENTQUEUE_HPP
