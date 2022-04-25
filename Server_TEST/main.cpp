#include <iostream>
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <thread>
#include <chrono>
#include <random>

#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

static zmq::context_t ctx;

int main( void )
{
    zmq::socket_t sock(ctx, zmq::socket_type::req);
    sock.connect("tcp://benternet.pxl-ea-ict.be:24041");

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> udist(0,100);

    while(1)
    {
        std::string msg_out = std::to_string(udist(mt));
        zmq::message_t z_out(msg_out);
        sock.send(z_out, zmq::send_flags::none);

        zmq::message_t (z_in);
        sock.recv(z_in);
        std::cout<< "\nsending: " << msg_out
                 << "received: " << z_in.to_string_view();

        sleep(1000);
    }
}
