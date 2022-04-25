#include <iostream>
#include <zmq.hpp>
#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

int main( void )
{
    try
    {
        zmq::context_t context(1);

        //Outgoing message go out through here
        zmq::socket_t Server( context, ZMQ_PUSH );

        Server.connect( "tcp://benternet.pxl-ea-ict.be:24041" );
        std::cout << "connect" << std::endl;

//        while( Server.connected() )
//        {
//            sleep( 2000 );
//            Server.send( "Yahtzee>", 10 );
//            std::cout << "Verzonden" << std::endl;
//        }
        zmq::message_t * msg = new zmq::message_t();
        while( Server.connected() )
        {
            Server.recv( msg );
            std::string s( (char*) msg->data(), msg->size() );
            std::cout << "Received : [" << s << "]" << std::endl;
            Server.send( *msg );
            std::cout << "Replied : [" << s << "]" << std::endl;
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Error! : " << ex.what();
    }

    return 0;
}
