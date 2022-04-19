#include <iostream>
#include <string>
#include <zmq.hpp>

int main( void )
{
    try
    {
        zmq::context_t context(1);

        //Incoming messages come in here
        zmq::socket_t client( context, ZMQ_SUB );
        client.connect( "tcp://benternet.pxl-ea-ict.be:24042" );
        client.setsockopt( ZMQ_SUBSCRIBE, "Yahtzee>", 8 );

        zmq::message_t * msg = new zmq::message_t();
        while( client.connected() )
        {
            client.recv( msg );
            std::cout << "Subscribed : " << std::string( (char*) msg->data(), msg->size() ) << std::endl;

        }
    }
    catch( zmq::error_t & error )
    {
        std::cerr << "Error! : " << error.what();
    }
    return 0;
}
