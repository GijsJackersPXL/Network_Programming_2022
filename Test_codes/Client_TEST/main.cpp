 #include <iostream>
#include <string>
//#include <zmq.hpp>
#include <zmq_addon.hpp>

static zmq::context_t ctx;

int main( void )
{
    try
    {
        zmq::socket_t sock(ctx, zmq::socket_type::rep);
        sock.bind("tcp://127.0.0.1:5555");

        while ( sock.connected() ) {
            zmq::message_t z_in;
            sock.recv(z_in);

            int x = std::stoi(z_in.to_string());
            x = x * x;

            std::string msg_out = std::to_string(x);
            std::cout << "\nsending back: " << msg_out;
            zmq::message_t z_out(msg_out);
            sock.send(z_out, zmq::send_flags::none);
        }
    }
    catch( zmq::error_t & error )
    {
        std::cerr << "Error! : " << error.what();
    }
}
