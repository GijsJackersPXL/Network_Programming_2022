#include <iostream>
#include <zmq_addon.hpp>

static zmq::context_t ctx;

int main( void )
{
   try
   {
       zmq::socket_t sock(ctx, zmq::socket_type::sub);
       sock.bind("tcp://127.0.0.1:5555");

       {
           std::vector<std::string> topics{ "speler1", "speler2"};
           for (auto&& topic : topics)
               sock.setsockopt(ZMQ_SUBSCRIBE, topic.data(),topic.size());
       }


       while (1) {
           zmq::message_t topic;
           zmq::message_t payload;
           sock.recv(topic);
           sock.recv(payload);

           std::cout << "\n" << topic.to_string_view() <<
                        " :" << payload.to_string_view();
       }
   }
   catch( zmq::error_t & error )
   {
       std::cerr << "Error! : " << error.what();
   }
}
