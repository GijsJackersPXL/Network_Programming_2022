#include <iostream>
#include <string>
#include <zmq.hpp>

int main(void)
{
    try
    {

        std::string sendtpic("GijsJackers>Yahtzee?>");

        // Incoming messages come in here
        zmq::context_t context(1);
        zmq::socket_t pusher(context, ZMQ_PUSH);
        zmq::socket_t subscriber(context, ZMQ_SUB);

        pusher.connect("tcp://benternet.pxl-ea-ict.be:24041");
        subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");

        subscriber.setsockopt(ZMQ_SUBSCRIBE, NULL, 0);

        zmq::message_t *msg = new zmq::message_t();

        // pusher.send( "example>quest?>Toon De Leersnyder>", 38 );

        std::string service;

        while (subscriber.connected())
        {

            std::cin >> service;
            pusher.send(service.c_str(), service.length());
            // subscriber.recv( msg );

            // std::cout << "Subscribed : [" << std::string( (char*) msg->data(), msg->size() ) << "]" << std::endl;
            // printf("yeet3");
        }
    }
    catch (zmq::error_t &ex)
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

    return 0;
}
