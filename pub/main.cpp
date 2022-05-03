#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <nlohmann/json.hpp>
#include <zmq_addon.hpp>

static zmq::context_t ctx;

using namespace std;

int main()
{
    zmq::socket_t sock(ctx, zmq::socket_type::pub);
    sock.connect("tcp://127.0.0.1:5555");

    std::random_device rd;
    std::mt19937 mt(rd());
    std::normal_distribution<double> normdist(0.,0.1);
    std::uniform_int_distribution<int> udist(0,2);

    while(1)
    {
        std::this_thread::sleep_for(200ms);

        std::string name = "";
        nlohmann::json jmsg;
        auto randint = udist(mt);

        if (randint == 0){
            name = "speler1";
            jmsg[name] = normdist(mt);
        }
        else if (randint == 1){
            name = "speler2";
            jmsg[name] = normdist(mt);
        }
        else {
            name = "admin";
            jmsg[name] = normdist(mt);
        }

        std::cout << '\n' << jmsg.dump();

        //topic
        zmq::message_t topic(name);
        sock.send(topic, zmq::send_flags::sndmore);

        //main payload
        zmq::message_t payload(jmsg.dump());
        sock.send(payload,zmq::send_flags::none);

    }
}
