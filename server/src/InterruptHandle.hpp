#ifndef SMP_INTERRUPTHANDLE_HPP
#define SMP_INTERRUPTHANDLE_HPP

class InterruptHandle {
public:
    InterruptHandle() {
        this->interrupted = false;
    }

    void interrupt() {
        this->interrupted = true;
    }

    bool isInterrupted() {
        return this->interrupted;
    }

private:
    bool interrupted;
};

#endif //SMP_INTERRUPTHANDLE_HPP
