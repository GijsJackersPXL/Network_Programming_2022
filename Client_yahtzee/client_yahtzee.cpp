#include "client_yahtzee.h"

client::client()
{
}

void client::startClient()
{
    try
    {
        std::string SubTopic("GijsJackers>Yahtzee?>");

        zmq::context_t context(1);

        //PUB
        zmq::socket_t pusher(context, ZMQ_PUB);
        pusher.connect( "tcp://127.0.0.1:5555" );

        //SUB
        zmq::socket_t subscriber(context, ZMQ_SUB);
        subscriber.bind("tcp://127.0.0.1:5556");
        subscriber.setsockopt(ZMQ_SUBSCRIBE, SubTopic.data(), SubTopic.length());

        zmq::message_t *msg = new zmq::message_t();

        while(pusher.connected())
        {
            // -- STARTEN VAN YAHTZEE -- //

            std::cout << "Starten van Yahtzee?" << std::endl << "> ";
            scanf("%100s", starten);

            if (!strcmp(starten, "y"))
            {
                std::cout << std::endl;
                std::string sendTopic("GijsJackers>Yahtzee?>Start");
                pusher.send(sendTopic.c_str(), sendTopic.length());

                std::string strGegooideDobbelstenen;
                int arrGegooideDobbelstenen[5];

                subscriber.recv(msg);

                if (strncmp((char*) msg->data(), "GijsJackers>Yahtzee?>Dobbelstenen", 33) == 0)
                {
                    std::string sendData = std::string((char *)msg->data());
                    strGegooideDobbelstenen = sendData.substr(sendData.find("?>Dobbelstenen") + 14, 5);
                }
                arrGegooideDobbelstenen[5];
                std::cout << "De gegooide dobbelstenen zijn: ";

                for (int i = 0; i < 5; i++)
                {
                    arrGegooideDobbelstenen[i] = stoi(strGegooideDobbelstenen.substr(i, 1));
                    std::cout << arrGegooideDobbelstenen[i] << " ";
                }
                std::cout << std::endl << std::endl;


                // -- DOBBELSTENEN OPNIEUW GOOIEN --//

                std::cout << "Wilt u de dobbelstenen opnieuw gooien?" << std::endl << "> ";
                scanf("%100s", opnieuwgooien);
                if (!strcmp(opnieuwgooien, "y"))
                {
                    std::cout << std::endl;
                    std::cout << "Welke dobbelsteen wilt u opnieuw gooien? (1>2>X>4>X)?" << std::endl << "> ";
                    scanf("%100s", gekozendobbelstenen);

                    sendTopic = "GijsJackers>Yahtzee?>DobbelstenenOpnieuw";
                    sendTopic += gekozendobbelstenen;
                    sendTopic += ">>" + strGegooideDobbelstenen ;
                    pusher.send(sendTopic.c_str(), sendTopic.length());

                    subscriber.recv(msg);

                    if (strncmp((char*) msg->data(), "GijsJackers>Yahtzee?>DobbelstenenOpnieuwGegooid", 47) == 0)
                    {
                        std::string sendData = std::string((char *)msg->data());
                        std::string strOpnieuwGegooideDobbelstenen = sendData.substr(sendData.find("?>DobbelstenenOpnieuwGegooid") + 28, 5);
                        std::cout << std::endl << "De gegooide dobbelstenen zijn: ";

                        for (int i = 0; i < 5; i++)
                        {
                            arrGegooideDobbelstenen[i] = stoi(strOpnieuwGegooideDobbelstenen.substr(i, 1));
                            std::cout << arrGegooideDobbelstenen[i] << " ";
                        }
                        std::cout << std::endl << std::endl;
                    }
                }
                else
                {
                    std::cout << std::endl << "Dobbelstenen niet opnieuw gooien. " << std::endl << std::endl;
                }
            }
            std::cout << "EINDE-----------------------------------------------------------------------" << std::endl << std::endl;
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Error!!! : " << ex.what();
    }
}

