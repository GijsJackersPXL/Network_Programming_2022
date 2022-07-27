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

};
#endif // SERVER_YAHTZEE_H
