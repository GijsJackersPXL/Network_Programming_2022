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

        while( Server.connected() )
        {
            sleep( 2000 );
            Server.send( "Yahtzee>", 10 );
            std::cout << "Verzonden" << std::endl;

//            for (int i = 0; i < 10; ++i)
//            {
//                Server.send("Message %d\n",12);
//                //std::cout << "Verzonden" << std::endl;
//            }
//            std::cout << "na for" << std::endl;
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Error! : " << ex.what();
    }

    return 0;
}
