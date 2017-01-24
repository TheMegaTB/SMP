//
// Created by themegatb on 1/3/17.
//

#include "Channel.hpp"

int* Channel::getAddress() {
    return this->address;
}

string Channel::getAddressAsString() {
    string addr = to_string(this->address[0]);
    addr += "-" + to_string(this->address[1]);
    addr += "-" + to_string(this->address[2]);
    return addr;
}

void Channel::setAddress(vector<int>* address) {
    this->address[0] = (*address)[0];
    this->address[1] = (*address)[1];
    this->address[2] = (*address)[2];
}

Channel::Channel(vector<int>* channel) {
    this->setAddress(channel);
}

Channel::Channel(int type, int room, int device) {
    this->address[0] = type;
    this->address[1] = room;
    this->address[2] = device;
}