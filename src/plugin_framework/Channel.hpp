//
// Created by themegatb on 1/3/17.
//

#ifndef SMARTHOME_CHANNEL_HPP
#define SMARTHOME_CHANNEL_HPP

#include <vector>

using namespace std;

class Channel {
    int type;
    int room;
    int device;
public:
    vector<int> getAddress() {
        vector<int> address = {this->type, this->room, this->device};
        return address;
    }

    string getAddressAsString() {
        string addr = to_string(this->type);
        addr += "-" + to_string(this->room);
        addr += "-" + to_string(this->device);
        return addr;
    }

    void setAddress(vector<int> address) {
        this->type = address[0];
        this->room = address[1];
        this->device = address[2];
    }

    Channel(vector<int> channel) {
        this->setAddress(channel);
    }

    Channel(int type, int room, int device) {
        this->type = type;
        this->room = room;
        this->device = device;
    }
};


#endif //SMARTHOME_CHANNEL_HPP
