#include "server_yahtzee.h"

Server::Server(): context(1), pusher(context, ZMQ_PUSH), subscriber(context, ZMQ_SUB)
{
}

void Server::serverStart()
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

           std::string SubTopic( "GijsJackers>Yahtzee?>" );
           subscriber.setsockopt(ZMQ_SUBSCRIBE, SubTopic.data(), SubTopic.length());
           //subscriber.setsockopt(ZMQ_SUBSCRIBE, NULL, 0);

           zmq::message_t *msg = new zmq::message_t();

           while (subscriber.connected())
           {
//               std::cout << "connected" << std::endl;
               subscriber.recv(msg);
               std::cout << std::string( (char*) msg->data(), msg->size() ) << std::endl;

               // -- STARTEN VAN YAHTZEE -- //
               if (strncmp((char*) msg->data(), "GijsJackers>Yahtzee?>Naam", 25) == 0)
               {
                   beurtNr++;
                   std::string sendData = std::string((char *)msg->data());
                   std::string strNaam = sendData.substr(sendData.find("?>Naam") + 6, (sendData.find(">>")   - (sendData.find("?>Naam") + 6)));
                   strcpy(naamSpeler, strNaam.c_str());
                   std::cout << "Naam van de speler: " << naamSpeler << " BeurtNr: " << beurtNr << std::endl;

                   scoreBordNamen[beurtNr] = naamSpeler;
                   opnieuwgegooid = false;

               }

               else if (strncmp((char*) msg->data(), "GijsJackers>Yahtzee?>Start", 26) == 0)
               {
                   std::cout << "-- Start Yahtzee, Rol de dobbelstenen voor " << naamSpeler << std::endl << "Zijn dobbelstenen zijn:";
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
                   std::cout << std::endl << "-- Rol dobbelsteen ";
                   std::string sendData = std::string((char *)msg->data());
                   std::string strKeuzeOpnieuwGooien = sendData.substr(sendData.find("?>DobbelstenenOpnieuw") + 21, 9);
                   std::string strGegooideDobbelstenen = sendData.substr(sendData.find(">>") + 2, 5);

                   opnieuwgegooid = true;

                   // X eruit halen
                   std::string strArrKeuzenOpnieuwGooien[5];
                   std::stringstream string_streamKeuze(strKeuzeOpnieuwGooien);
                   int lengteArray = (sizeof(strArrKeuzenOpnieuwGooien) / sizeof(std::string));

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
                   for (unsigned int i = 0; i < strGegooideDobbelstenen.length(); i++)
                   {
                       strArrGegooideDobbelstenen[i] = strGegooideDobbelstenen[i];
                   }

                   for (int i = 0; i < lengteArray; i++)
                   {
                       if (strArrKeuzenOpnieuwGooien[i] == "X" || strArrKeuzenOpnieuwGooien[i] == "x")
                       {
                           std::cout << "'" << i + 1 << "' ";
                           strArrGegooideDobbelstenen[i] = gooiDobbelsteen() + '0';
                       }
                   }

                   std::cout << "opnieuw voor " << naamSpeler << std::endl << "De dobbelstenen zijn:";

                   std::string sendTopic("GijsJackers>Yahtzee?>DobbelstenenOpnieuwGegooid");
                   for (int i = 0; i < 5; i++)
                   {
                       sendTopic += strArrGegooideDobbelstenen[i];
                       std::cout << " " << strArrGegooideDobbelstenen[i];
                   }
                   pusher.send(sendTopic.c_str(), sendTopic.length());
                   std::cout << std::endl << std::endl;
               }
               else if (strncmp((char*) msg->data(), "GijsJackers>Yahtzee?>printTabel", 31) == 0)
               {
                   printTabel();
                   scoreBordPunten[beurtNr] = getTotaalPuntenSpeler();
               }
               else if (strncmp((char*) msg->data(), "GijsJackers>Yahtzee?>printScoreBord>", 36) == 0)
               {
                   printScoreBord();
               }
               else if (strncmp((char*) msg->data(), "GijsJackers>Yahtzee?>berekenenMogelijkePunten>", 36) == 0)
               {
                   berekenenMogelijkePunten();
               }

               std::cout << std::endl;
           }
       }
       catch( zmq::error_t & error )
       {
           std::cerr << "Error!!!: " << error.what();
       }
}

int Server::gooiDobbelsteen()
{
    return (rand() % 6) + 1 ;
}


void Server::printTabel()
{
    std::cout << "Printen van Tabel" << std::endl;

    somSpeler = 0;
    bonusSpeler = 0;
    totaalPuntenSpeler = 0;

    for (int i = 0; i <= 5; i++)
    {
        somSpeler += puntenSpeler[i];
    }

    if (somSpeler >= 63)
    {
        bonusSpeler = 35;
    }

    for (int i = 6; i < 13; i++)
    {
        totaalPuntenSpeler += puntenSpeler[i];
    }
    totaalPuntenSpeler += bonusSpeler + somSpeler;

    std::string beginletterSpeler(1,naamSpeler[0]);

    arrPrintTabel[0] = " --------------------";
    arrPrintTabel[1] = "| Speler         | " + beginletterSpeler + " |";
    arrPrintTabel[2] = " --------------------";
    arrPrintTabel[3] = "| Enen           | " + std::to_string(puntenSpeler[0]) + " |";
    arrPrintTabel[4] = " --------------------";
    arrPrintTabel[5] = "| Tweeen         | " + std::to_string(puntenSpeler[1]) + " |";
    arrPrintTabel[6] = " --------------------";
    arrPrintTabel[7] = "| Drieen         | " + std::to_string(puntenSpeler[2]) + " |";
    arrPrintTabel[8] = " --------------------";
    arrPrintTabel[9] = "| Vieren         | " + std::to_string(puntenSpeler[3]) + " |";
    arrPrintTabel[10] = " --------------------";
    arrPrintTabel[11] = "| Vijven         | " + std::to_string(puntenSpeler[4]) + " |";
    arrPrintTabel[12] = " --------------------";
    arrPrintTabel[13] = "| Zessen         | " + std::to_string(puntenSpeler[5]) + " |";
    arrPrintTabel[14] = " --------------------";
    arrPrintTabel[15] = "| SOM            | " + std::to_string(somSpeler) + " |";
    arrPrintTabel[16] = " --------------------";
    arrPrintTabel[17] = "| BONUS          | " + std::to_string(bonusSpeler) + " |";
    arrPrintTabel[18] = " --------------------";
    arrPrintTabel[19] = "| Drie gelijke   | " + std::to_string(puntenSpeler[6]) + " |";
    arrPrintTabel[20] = " --------------------";
    arrPrintTabel[21] = "| Vier gelijke   | " + std::to_string(puntenSpeler[7]) + " |";
    arrPrintTabel[22] = " --------------------";
    arrPrintTabel[23] = "| Kleine straat  | " + std::to_string(puntenSpeler[8]) + " |";
    arrPrintTabel[24] = " --------------------";
    arrPrintTabel[25] = "| Grote straat   | " + std::to_string(puntenSpeler[9]) + " |";
    arrPrintTabel[26] = " --------------------";
    arrPrintTabel[27] = "| Full House     | " + std::to_string(puntenSpeler[10]) + " |";
    arrPrintTabel[28] = " --------------------";
    arrPrintTabel[29] = "| Kans           | " + std::to_string(puntenSpeler[11]) + " |";
    arrPrintTabel[30] = " --------------------";
    arrPrintTabel[31] = "| Yahtzee        | " + std::to_string(puntenSpeler[12]) + " |";
    arrPrintTabel[32] = " --------------------";
    arrPrintTabel[33] = "| TOTAAL         | " + std::to_string(totaalPuntenSpeler) + " |";
    arrPrintTabel[34] = " --------------------";

    std::string doorsturen;
    int lengteArray = (sizeof(arrPrintTabel) / sizeof(std::string));

    for (int i = 0; i < 35; i++)
    {
        std::string sendTopic = "GijsJackers>Yahtzee?>printTabel>";

        doorsturen = arrPrintTabel[i] + ">>";
        sendTopic += doorsturen;
        pusher.send(sendTopic.c_str(), sendTopic.length());
    }

    std::string sendTopic = "GijsJackers>Yahtzee?>printTabel>EindePrintTabel>";
    pusher.send(sendTopic.c_str(), sendTopic.length());
}


int Server::getTotaalPuntenSpeler()
{
    int somSpeler1 = 12;
    int bonusSpeler1 = 3;
    int totaalPuntenSpeler = 0;

    for(int i = 0; i <= 5; i++)
    {
        somSpeler1 += puntenSpeler1[i];
    }

    if(somSpeler1 >= 63)
    {
        bonusSpeler1 = 35;
    }

    for(int i = 6; i < 13; i++)
    {
        totaalPuntenSpeler += puntenSpeler1[i];
    }
    totaalPuntenSpeler += bonusSpeler1 + somSpeler1;

    return totaalPuntenSpeler;
}


void Server::printScoreBord()
{
    std::cout << std::endl << "Printen van scorebord" << std::endl;

    std::string doorsturen;
    int lengteArray = (sizeof(scoreBordNamen) / sizeof(std::string));

    for (int i = 0; i < lengteArray; i++)
    {
        std::string sendTopic = "GijsJackers>Yahtzee?>printScoreBord>";

        if (scoreBordNamen[i] != "")
        {
            doorsturen = " " + scoreBordNamen[i] + ": " + std::to_string(scoreBordPunten[i]) + ">>";
            sendTopic += doorsturen;
            pusher.send(sendTopic.c_str(), sendTopic.length());
        }
    }

    std::string sendTopic = "GijsJackers>Yahtzee?>printScoreBord>EindePrintScoreBord>";
    pusher.send(sendTopic.c_str(), sendTopic.length());

}

void Server::berekenenMogelijkePunten()
{
    if (opnieuwgegooid == true)
    {
        for(int i = 0; i < 5; i++)
        {
            intArrGegooideDobbelstenen[i] = std::atoi(strArrGegooideDobbelstenen[i].c_str());
            std::cout << strArrGegooideDobbelstenen[i] << std::endl;
        }
    }
    else
    {
        for(int i = 0; i < 5; i++)
        {
            intArrGegooideDobbelstenen[i] = charArrGegooideDobbelstenen[i] - '0';
            std::cout << charArrGegooideDobbelstenen[i] << std::endl;
        }
    }

    for(int i = 0; i < 5; i++)
    {
        switch (intArrGegooideDobbelstenen[i])
        {
            case 1:
                mogelijkePunten[0] += 1;
                break;
            case 2:
                mogelijkePunten[1] += 2;
                break;
            case 3:
                mogelijkePunten[2] += 3;
                break;
            case 4:
                mogelijkePunten[3] += 4;
                break;
            case 5:
                mogelijkePunten[4] += 5;
                break;
            case 6:
                mogelijkePunten[5] += 6;
                break;
            default:
                std::cout << "Fout in berekenDobbelstenenBeurt" << std::endl;
                break;
        }
    }

    //gelijke getallen
    int tweeDezelfde;
    int teller;
    for(int i = 0; i < 5; i++)
    {
        tweeDezelfde = 0;
        teller = 0;
        for(int j = 0; j < 5; j++)
        {
            if(intArrGegooideDobbelstenen[i] == intArrGegooideDobbelstenen[j])
            {
                teller++;
            }
            else
            {
                if(intArrGegooideDobbelstenen[j] != tweeDezelfde && tweeDezelfde != 0)
                {
                    tweeDezelfde = 0;
                }
                else
                {
                    tweeDezelfde = intArrGegooideDobbelstenen[j];
                }
            }
        }

        //drieGelijke
        if(teller >= 3)
        {
            mogelijkePunten[6] = somOgen();
        }

        //vierGelijke
        if(teller >= 4)
        {
            mogelijkePunten[7] = somOgen();
        }

        //fullHouse
        if(teller == 3 && tweeDezelfde != 0)
        {
            mogelijkePunten[10] = 25;
        }
    }

    //kleineStraat
    //((drieen && vieren) && ((enen && tweeen) || (tweeen && vijven) || (vijven && zessen))
    if((mogelijkePunten[2] && mogelijkePunten[3]) && ((mogelijkePunten[0] && mogelijkePunten[1]) || (mogelijkePunten[1] && mogelijkePunten[4]) || (mogelijkePunten[4] && mogelijkePunten[5])))
    {
        mogelijkePunten[8] = 30;
    }

    //groteStraat
    //((tweeen && drieen && vieren && vijven) && (enen || zessen)
    if((mogelijkePunten[1] && mogelijkePunten[2] && mogelijkePunten[3] && mogelijkePunten[4]) && (mogelijkePunten[0] || mogelijkePunten[5]))
    {
        mogelijkePunten[9] = 40;
    }

    //kans
    mogelijkePunten[11] = somOgen();

    //yahtzee
    if(teller == 5)
    {
        mogelijkePunten[12] = 50;

    }

    std::cout << std::endl << "Printen van mogelijke punten" << std::endl;
    std::string doorsturen;

    for(int i = 0; i < 13; i++)
    {
        std::string sendTopic = "GijsJackers>Yahtzee?>printMogelijkePunten>";
        if(gekozenPuntenSpeler1[i] == 0) //als punt al gekozen is wordt deze niet meer geprint
        {
            //std::cout << "|------------------------| \n| " << std::to_string(i+1) << printScoreNamen(i+1) << mogelijkePunten[i] << std::endl;

            doorsturen = "|------------------------| \n| " + std::to_string(i+1) + ". " + printScoreNamen(i+1) + std::to_string(mogelijkePunten[i]) + " |>>";
            sendTopic += doorsturen;
            pusher.send(sendTopic.c_str(), sendTopic.length());
        }

    }
    std::string sendTopic = "GijsJackers>Yahtzee?>printMogelijkePunten>";
    doorsturen = " ------------------------->>";
    sendTopic += doorsturen;
    pusher.send(sendTopic.c_str(), sendTopic.length());

    sendTopic = "GijsJackers>Yahtzee?>printTabel>EindePrintMogelijkePunten>";
    pusher.send(sendTopic.c_str(), sendTopic.length());

}

int Server::somOgen()
{
    int som = 0;
    for(int i = 0; i < 5; i++)
    {
        som += intArrGegooideDobbelstenen[i];
    }
    return som;
}

std::string Server::printScoreNamen(int getal)
{
    std::string returnNaam;
    switch (getal)
    {
        case 1:
            returnNaam = "Enen            | ";
            break;
        case 2:
            returnNaam = "Tweeen          | ";
            break;
        case 3:
            returnNaam = "Drieen          | ";
            break;
        case 4:
            returnNaam = "Vieren          | ";
            break;
        case 5:
            returnNaam = "Vijven          | ";
            break;
        case 6:
            returnNaam = "Zessen          | ";
            break;
        case 7:
            returnNaam = "Drie gelijke    | ";
            break;
        case 8:
            returnNaam = "Vier gelijke    | ";
            break;
        case 9:
            returnNaam = "Kleine straat   | ";
            break;
        case 10:
            returnNaam = "Grote straat   | ";
            break;
        case 11:
            returnNaam = "Full House     | ";
            break;
        case 12:
            returnNaam = "Kans           | ";
            break;
        case 13:
            returnNaam = "Yahtzee        | ";
            break;
        default:
            returnNaam = "Fout in printScoreNamen!";
            break;
    }
    return returnNaam;
}

















