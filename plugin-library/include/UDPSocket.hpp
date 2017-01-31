#ifndef UDP_SOCKET_HPP
#define UDP_SOCKET_HPP

#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::udp;

class UDPSocket {
    boost::asio::io_service service;
    udp::socket socket;
    udp::endpoint destination;
public:
    UDPSocket(std::string multicast_group, unsigned long port);

    int recv(std::string *msg, unsigned int timeout_ms);

    void send(std::string message);
};

#endif //UDP_SOCKET_HPP
