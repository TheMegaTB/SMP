//
// Created by themegatb on 12/30/16.
//

#ifndef SMARTHOME_INTERRUPTHANDLE_HPP
#define SMARTHOME_INTERRUPTHANDLE_HPP

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

#endif //SMARTHOME_INTERRUPTHANDLE_HPP
