//
// Created by themegatb on 12/27/16.
//

#ifndef SMP_SafeQueue_HPP
#define SMP_SafeQueue_HPP

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

    int take(T *val, std::chrono::duration<int, std::milli> timeout) {
        std::unique_lock<std::mutex> lock(m);
        while (q.empty()) {
            if (c.wait_for(lock, timeout) == std::cv_status::timeout) return -1;
        }
        *val = q.front();
        q.pop();
        return 1;
    }

private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
};

#endif //SMP_SafeQueue_HPP
