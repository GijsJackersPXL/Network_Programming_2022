#ifndef CLIENT_YAHTZEE_H
#define CLIENT_YAHTZEE_H
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "zmq.hpp"
#include <QString>

class Client
{
    public:
        Client();
        void startClient();
        void printTabel();
        void printScoreNamen(int);

    private:
        zmq::context_t context;
        zmq::socket_t pusher;
        zmq::socket_t subscriber;

        int arrGegooideDobbelstenen[5];
        std::string strGegooideDobbelstenen;

        char starten[101];
        char opnieuwgooien[101];
        char berekenenPunten[101];
        char gekozendobbelstenen[101];

        char naamSpeler1[20] = "";
        char scoreBord[1];

//        int puntenSpeler[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


};

#endif // CLIENT_YAHTZEE_H
