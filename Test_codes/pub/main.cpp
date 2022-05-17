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
    zmq::socket_t sock1(ctx, zmq::socket_type::pub);
    sock1.connect("tcp://127.0.0.1:5555");

    zmq::socket_t sock2(ctx, zmq::socket_type::pub);
    sock2.connect("tcp://127.0.0.1:5556");

    std::random_device rd;
    std::mt19937 mt(rd());
    std::normal_distribution<double> normdist(0.,0.1);
    std::uniform_int_distribution<int> udist(0,2);

    while(1)
    {
        std::this_thread::sleep_for(200ms);

        std::string name = "";
        std::string name2 = "";
        nlohmann::json jmsg;
        auto randint = udist(mt);

        if (randint == 0){
            name = "speler1";
            jmsg[name] = normdist(mt);
        }
        else if (randint == 1){
            name2 = "speler2";
            jmsg[name2] = normdist(mt);
        }
        else {
            name = "admin";
            jmsg[name] = normdist(mt);
        }

        std::cout << '\n' << jmsg.dump();

        //topic
        zmq::message_t topic1(name);
        sock1.send(topic1, zmq::send_flags::sndmore);

        //main payload
        zmq::message_t payload1(jmsg.dump());
        sock1.send(payload1,zmq::send_flags::none);

        //topic
        zmq::message_t topic2(name2);
        sock2.send(topic2, zmq::send_flags::sndmore);

        //main payload
        zmq::message_t payload2(jmsg.dump());
        sock2.send(payload2,zmq::send_flags::none);

    }
}

We starten het spel\n
