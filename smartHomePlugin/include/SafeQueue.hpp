//
// Created by themegatb on 12/27/16.
//

#ifndef SMARTHOME_SafeQueue_HPP
#define SMARTHOME_SafeQueue_HPP

#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>

template<class T>
class SafeQueue {
public:
    SafeQueue(void)
            : q(), m(), c() {}

    ~SafeQueue(void) {}

    void add(T t) {
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
        c.notify_one();
    }

    T take(std::chrono::duration<int, std::milli> timeout) { // TODO: Implement timeout
        std::unique_lock<std::mutex> lock(m);
        while (q.empty()) {
            if (c.wait_for(lock, timeout) == std::cv_status::timeout) return nullptr;
        }
        T val = q.front();
        q.pop();
        return val;
    }

private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
};

//template class SafeQueue<std::string>;
#endif //SMARTHOME_SafeQueue_HPP
