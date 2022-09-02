#ifndef SERVER_YAHTZEE_H
#define SERVER_YAHTZEE_H
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <zmq.hpp>

class Server
{
    public:
        Server();

        void serverStart();
        void dobbelstenenGooien();
        void naam( std::string );
        void dobbelstenenOpnieuwGooien( std::string );
        void checkScoreBord();
        void printScoreBord();
        void printTabel();
        void berekenenMogelijkePunten();

        int getTotaalPuntenSpeler();
        int gooiDobbelsteen();
        int somOgen();

        std::string getPuntenSpeler();
        std::string printScoreNamen(int);


    private:
        zmq::context_t context;
        zmq::socket_t pusher;
        zmq::socket_t subscriber;

        bool opnieuwgegooid = false;
        bool abortDobbelstenenOpnieuw = true;
        bool abortPrintTabel = true;
        bool abortPrintScoreBord = true;
        bool abortBerekenenMogelijkePunten = true;

        char charArrGegooideDobbelstenen[5];
        char naamSpeler[20] = "";
        char puntenSpeler[101];

        int puntenSpeler1[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int mogelijkePunten[13];
        int beurtNr = 0;
        int scoreBordPunten[101];
        int somSpeler = 0;
        int bonusSpeler = 0;
        int totaalPuntenSpeler = 0;
        int intArrGegooideDobbelstenen[5];
        int gekozenPuntenSpeler1[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        std::string strArrGegooideDobbelstenen[5];
        std::string scoreBordNamen[101];
        std::string arrPrintTabel[35];

        zmq::message_t *msg = new zmq::message_t();


};
#endif // SERVER_YAHTZEE_H
