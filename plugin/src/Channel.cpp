//
// Created by themegatb on 1/3/17.
//

#include "Channel.hpp"

vector<int> Channel::getAddress() {
    vector<int> address = {this->type, this->room, this->device};
    return address;
}

string Channel::getAddressAsString() {
    string addr = to_string(this->type);
    addr += "-" + to_string(this->room);
    addr += "-" + to_string(this->device);
    return addr;
}

void Channel::setAddress(vector<int> address) {
    this->type = address[0];
    this->room = address[1];
    this->device = address[2];
}

Channel::Channel(vector<int> channel) {
    this->setAddress(channel);
}

Channel::Channel(int type, int room, int device) {
    this->type = type;
    this->room = room;
    this->device = device;
}