#include "server_yahtzee.h"

int gooiDobbelsteen();

Server::Server()
{

}

void Server::serverStart()
{
    srand(time(0));
    try
       {
           std::string SubTopic( "GijsJackers>Yahtzee?>" );

           zmq::context_t context(1);

           //SUB
           zmq::socket_t subscriber(context, ZMQ_SUB);
           subscriber.bind("tcp://127.0.0.1:5555");
           subscriber.setsockopt(ZMQ_SUBSCRIBE, SubTopic.data(), SubTopic.length());

           //PUB
           zmq::socket_t pusher(context, ZMQ_PUB);
           pusher.connect( "tcp://127.0.0.1:5556" );

           zmq::message_t *msg = new zmq::message_t();

           while (subscriber.connected())
           {
               subscriber.recv(msg); 
               char charArrGegooideDobbelstenen[5];

               // -- STARTEN VAN YAHTZEE -- //
               if (strncmp((char*) msg->data(), "GijsJackers>Yahtzee?>Start", 26) == 0)
               {
                   std::cout << "-- Start Yahtzee, Rol de dobbelstenen" << std::endl << "De dobbelstenen zijn:";

                   for (int i = 0; i < 5; i++)
                   {
                      charArrGegooideDobbelstenen[i] = gooiDobbelsteen() + '0';
                      std::cout << " " << charArrGegooideDobbelstenen[i];
                   }
                   std::cout << std::endl;

                   std::string sendTopic("GijsJackers>Yahtzee?>Dobbelstenen");
                   for (int i = 0; i < 5; i++)
                   {
                       sendTopic += charArrGegooideDobbelstenen[i];
                   }
                   pusher.send(sendTopic.c_str(), sendTopic.length());
               }

               // -- DOBBELSTENEN OPNIEUW GOOIEN --//
               else if (strncmp((char*) msg->data(), "GijsJackers>Yahtzee?>DobbelstenenOpnieuw", 40) == 0)
               {
                   std::cout << "-- Rol de dobbelstenen opnieuw" << std::endl << "De dobbelstenen zijn:";
                   std::string sendData = std::string((char *)msg->data());
                   std::string strKeuzeOpnieuwGooien = sendData.substr(sendData.find("?>DobbelstenenOpnieuw") + 21, 9);
                   std::string strGegooideDobbelstenen = sendData.substr(sendData.find(">>") + 2, 5);

                   // X eruit halen
                   std::string strArrKeuzenOpnieuwGooien[5];
                   std::string strArrGegooideDobbelstenen[5];

                   std::stringstream string_streamKeuze(strKeuzeOpnieuwGooien);

                   int lengteArray = (sizeof(strArrKeuzenOpnieuwGooien) / sizeof(std::string));

                   int index = 0;
                   int nietGekozen = 0;

                   //1>2>X>4>X => {'1','2','X','4','X'}
                   //strKeuzeOpnieuwGooien => strArrKeuzenOpnieuwGooien
                   for (int i = 0; i < lengteArray; i++)
                   {
                       std::string substr;
                       getline(string_streamKeuze, substr, '>');
                       strArrKeuzenOpnieuwGooien[i] = substr;
                   }
                   //12345 => {'1','2','3','4','5'}
                   //strGegooideDobbelstenen => strArrGegooideDobbelstenen
                   for (int i = 0; i < strGegooideDobbelstenen.length(); i++)
                   {
                       strArrGegooideDobbelstenen[i] = strGegooideDobbelstenen[i];
                   }

                   for (int i = 0; i < lengteArray; i++)
                   {
                       if (strArrKeuzenOpnieuwGooien[i] == "X" || strArrKeuzenOpnieuwGooien[i] == "x")
                       {
                           strArrGegooideDobbelstenen[i] = gooiDobbelsteen() + '0';
                       }
                   }
//                   for (int i = 0; i < 5; i++)
//                   {
//                       if (arrGekozenDobbelstenen[i] != 9)
//                       {
//                           std::cout << arrGekozenDobbelstenen[i] << " ";
//                       }
//                       else
//                       {
//                            nietGekozen++;
//                       }
//                   }
//                   if (nietGekozen == 5)
//                   {
//                       std::cout << "Er worden geen dobbeltenen opnieuw gegooid" << std::endl;
//                   }
//                   else
//                   {
//                       std::cout << "zullen opnieuw gegooid worden" << std::endl;
//                   }

                   std::string sendTopic("GijsJackers>Yahtzee?>DobbelstenenOpnieuwGegooid");
                   for (int i = 0; i < 5; i++)
                   {
                       sendTopic += strArrGegooideDobbelstenen[i];
                       std::cout << " " << strArrGegooideDobbelstenen[i];
                   }
                   pusher.send(sendTopic.c_str(), sendTopic.length());
                   std::cout << std::endl << std::endl;
               }

               // -- KIEZEN VAN SCORE --//
           }
       }
       catch( zmq::error_t & error )
       {
           std::cerr << "Error!!! : " << error.what();
       }
}

int gooiDobbelsteen()
{
    return (rand() % 6) + 1 ;
}
