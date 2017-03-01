#ifndef SMP_INTERRUPTHANDLE_HPP
#define SMP_INTERRUPTHANDLE_HPP

#include <thread>
#include <vector>

class InterruptHandle {
private:
    bool interrupted;
public:
    std::vector<thread> handles;
    InterruptHandle() {
        this->interrupted = false;
    }

    void interrupt() {
        this->interrupted = true;
    }

    bool isInterrupted() {
        return this->interrupted;
    }

    void waitForFinish() {
        for (std::thread &handle : this->handles)
            if (handle.joinable()) handle.join();
    }
};

#endif //SMP_INTERRUPTHANDLE_HPP
