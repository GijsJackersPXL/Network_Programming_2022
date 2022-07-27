#include <stdio.h>
#include <iostream>
#include <string.h>
#include <zmq.hpp>
#include "server_yahtzee.h"

int main( void )
{
    Server * s1 = new Server();
    s1->serverStart();
}
