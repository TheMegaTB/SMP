//
// Created by themegatb on 1/3/17.
//

#ifndef SMARTHOME_CHANNEL_HPP
#define SMARTHOME_CHANNEL_HPP

#include <vector>
#include <string>

using namespace std;

class Channel {
//    int type;
//    int room;
//    int device;
    int address[3];
public:
    Channel(vector<int>* channel);

    Channel(int type, int room, int device);

    int* getAddress();

    string getAddressAsString();

    void setAddress(vector<int>* address);
};


#endif //SMARTHOME_CHANNEL_HPP
