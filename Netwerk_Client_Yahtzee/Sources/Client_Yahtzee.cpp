#include "client_yahtzee.h"

Client::Client(): context(1), pusher(context, ZMQ_PUSH), subscriber(context, ZMQ_SUB)
{
}

void Client::startClient()
{
    srand(time(0));
    try
    {
        //PUB
        //pusher.connect( "tcp://127.0.0.1:5555" );
        pusher.connect( "tcp://benternet.pxl-ea-ict.be:24041" );

        //SUB
        //subscriber.connect("tcp://127.0.0.1:5556");
        subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" );

        std::string SubTopic("GijsJackers>Yahtzee?>");
        subscriber.setsockopt(ZMQ_SUBSCRIBE, SubTopic.data(), SubTopic.length());

        zmq::message_t *msg = new zmq::message_t();

        while(pusher.connected())
        {
            // -- STARTEN VAN YAHTZEE -- //
            std::cout << "De naam van speler (MAX. 20 characters): " << std::endl;
            scanf("%20s",naamSpeler1);
            if (strcmp(naamSpeler1, ""))
            {
                naamSpeler1[0] = toupper(naamSpeler1[0]);

                //doorstruen naam
                std::string sendTopic = "GijsJackers>Yahtzee?>Naam";
                sendTopic += naamSpeler1;
                sendTopic += ">>";
                pusher.send(sendTopic.c_str(), sendTopic.length());

                subscriber.recv(msg);
                std::cout << std::string( (char*) msg->data(), msg->size() ) << std::endl;

//                std::cout << std::endl << naamSpeler1 << " wil je starten met Yahtzee? (y/n)" << std::endl << "> ";
//                scanf("%s", starten);

                std::cout << std::endl << " y = Yahtzee starten / s = Scorebord printen (y/n)" << std::endl << "> ";
                scanf("%s", starten);
            }

            if (!strcmp(starten, "y"))
            {
                // -- STARTEN YAHTZEE -- //
                std::cout << std::endl;
                std::string sendTopic("GijsJackers>Yahtzee?>Start");
                pusher.send(sendTopic.c_str(), sendTopic.length());

                //wachten tot "GijsJackers>Yahtzee?>Dobbelstenen"
                subscriber.recv(msg);
                std::cout << std::string( (char*) msg->data(), msg->size() ) << std::endl;
                subscriber.recv(msg);
                std::cout << std::string( (char*) msg->data(), msg->size() ) << std::endl;


                // -- DOBBELSTENEN GOOIEN -- //
                if (strncmp((char*) msg->data(), "GijsJackers>Yahtzee?>Dobbelstenen", 33) == 0)
                {
                    std::string sendData = std::string((char*) msg->data(), msg->size());
                    strGegooideDobbelstenen = sendData.substr(sendData.find("?>Dobbelstenen") + 14, 5);

                    std::cout << naamSpeler1 << " je dobbelstenen zijn: ";

                    for (int i = 0; i < 5; i++)
                    {
                        //arrGegooideDobbelstenen[i] = stoi(strGegooideDobbelstenen.substr(i, 1));
                        arrGegooideDobbelstenen[i] = QString::fromStdString(strGegooideDobbelstenen.substr(i, 1)).toInt();

                        std::cout << arrGegooideDobbelstenen[i] << " ";
                    }
                    std::cout << std::endl << std::endl;
                }


                // -- DOBBELSTENEN OPNIEUW GOOIEN -- //
                std::cout << naamSpeler1 << " wil je de dobbelstenen opnieuw gooien? (y/n)" << std::endl << "> ";
                scanf("%100s", opnieuwgooien);
                if (!strcmp(opnieuwgooien, "y"))
                {
                    std::cout << std::endl;
                    std::cout << "Welke dobbelsteen wil je opnieuw gooien? (1>2>X>4>X)?" << std::endl << "> ";
                    scanf("%100s", gekozendobbelstenen);

                    sendTopic = "GijsJackers>Yahtzee?>DobbelstenenOpnieuw";
                    sendTopic += gekozendobbelstenen;
                    sendTopic += ">>" + strGegooideDobbelstenen ;
                    pusher.send(sendTopic.c_str(), sendTopic.length());

                    subscriber.recv(msg);
                    std::cout << std::string( (char*) msg->data(), msg->size() ) << std::endl;
                    subscriber.recv(msg);
                    std::cout << std::string( (char*) msg->data(), msg->size() ) << std::endl;


                    if (strncmp((char*) msg->data(), "GijsJackers>Yahtzee?>DobbelstenenOpnieuwGegooid", 47) == 0)
                    {
                        std::string sendData = std::string((char*) msg->data(), msg->size());
                        std::string strOpnieuwGegooideDobbelstenen = sendData.substr(sendData.find("?>DobbelstenenOpnieuwGegooid") + 28, 5);
                        std::cout << std::endl << naamSpeler1 << " je dobbelstenen zijn: ";

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


                // -- PRINTEN MOGELIJKE PUNTEN -- //
                std::cout << naamSpeler1 << " wil je kijken welke punten je zou kunnen behalen? (y/n)" << std::endl << "> ";
                scanf("%100s", berekenenPunten);
                if (!strcmp(berekenenPunten, "y"))
                {

                    sendTopic = "GijsJackers>Yahtzee?>berekenenMogelijkePunten>";
                    pusher.send(sendTopic.c_str(), sendTopic.length());

                    subscriber.recv(msg);
                    std::string sendData = std::string((char*) msg->data(), msg->size());

                    while (sendData != "GijsJackers>Yahtzee?>EindePrintMogelijkePunten>")
                    {
                        std::cout << sendData.substr((sendData.find("?>printMogelijkePunten>") + 23), (sendData.find(">>")   - (sendData.find("?>printMogelijkePunten>") + 23))) << std::endl;
                        subscriber.recv(msg);
                        sendData = std::string((char*) msg->data(), msg->size());
                    }
                }
            }
            else if (!strcmp(starten, "s"))
            {
                std::string sendTopic = "GijsJackers>Yahtzee?>printScoreBord>";
                pusher.send(sendTopic.c_str(), sendTopic.length());

                subscriber.recv(msg);
                std::string sendData = std::string((char*) msg->data(), msg->size());
                std::cout << "sendData: " << sendData << std::endl;

                while (sendData != "GijsJackers>Yahtzee?>printScoreBord>EindePrintScoreBord>")
                {
                    std::cout << sendData.substr((sendData.find("?>printScoreBord>") + 17), (sendData.find(">>")   - (sendData.find("?>printScoreBord>") + 17))) << std::endl;
                    subscriber.recv(msg);
                    sendData = std::string((char*) msg->data(), msg->size());
                }
                std::cout << std::endl;
            }

            std::cout << "EINDE-----------------------------------------------------------------------" << std::endl << std::endl;
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Error!!! : " << ex.what();
    }
}
