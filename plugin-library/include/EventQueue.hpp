//
// Created by themegatb on 12/27/16.
//

#ifndef SMP_EVENTQUEUE_HPP
#define SMP_EVENTQUEUE_HPP

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
public:
    std::vector<Observer<T> *> observers;
    EventQueue(SafeQueue<T> *queue) : input_queue(queue) {};

    void addObserver(Observer<T> *o) {
        this->observers.push_back(o);
    };

    int observeOnce(std::chrono::duration<int, std::milli> timeout) {
        T event;
        if (this->input_queue->take(&event, timeout) > 0) return 1;
        for (Observer<T> *observer : this->observers)
            observer->process(event);
        return 0;
    };
};

#endif //SMP_EVENTQUEUE_HPP
