#ifndef CLIENT_YAHTZEE_H
#define CLIENT_YAHTZEE_H
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <zmq.hpp>
//#include <zmq_addon.hpp>


class client
{
public:
    client();
    void startClient();

    char starten[101];
    char opnieuwgooien[101];
    char gekozendobbelstenen[101];

};

#endif // CLIENT_YAHTZEE_H
