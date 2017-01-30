//
// Created by themegatb on 1/3/17.
//

#ifndef SMP_CHANNEL_HPP
#define SMP_CHANNEL_HPP

#include <vector>
#include <string>

using namespace std;

class Channel {
//    int type;
//    int room;
//    int device;
    int address[3];
public:
    Channel(string channel);

    Channel(vector<int>* channel);

    Channel(int type, int room, int device);

    vector<int> getAddress();

    string getAddressAsString();

    void setAddress(vector<int>* address);
};


#endif //SMP_CHANNEL_HPP
