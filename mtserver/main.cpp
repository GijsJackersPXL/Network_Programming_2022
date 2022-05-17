#include <pthread.h>
#include <unistd.h>
#include <cassert>
#include <string>
#include <iostream>
#include <zmq.hpp>
#include<stdio.h>


void *worker_routine (void *arg)
{
    zmq::context_t *context = (zmq::context_t *) arg;

    zmq::socket_t socket (*context, ZMQ_REP);
    socket.connect ("inproc://workers");

//    // Wait for next request from client
//    zmq::message_t request;
//    socket.recv (&request);
//    std::cout << "Received request: [" << (char*) request.data() << "]" << std::endl;

//    while (strcmp( (char*)request.data(), "Stop") != 0) {

//    }



    while (true) {
        // Wait for next request from client
        zmq::message_t request;
        socket.recv (&request);
        std::cout << "Received request: [" << (char*) request.data() << "]" << std::endl;

        // Do some 'work'
        sleep (1);

        if (strcmp( (char*)request.data(), "Start") == 0 ){
            // Send reply back to client
            zmq::message_t reply (21);
            memcpy ((void *) reply.data (), "We starten het spel", 21);
            socket.send (reply);
        }

        if (strcmp( (char*)request.data(), "GIJS") == 0 ){
            // Send reply back to client
            zmq::message_t reply (9);
            memcpy ((void *) reply.data (), "JACKERS", 9);
            socket.send (reply);
        }
    }
    return (NULL);
}


int main ()
{
    // Prepare our context and sockets
    zmq::context_t context (1);
    zmq::socket_t clients (context, ZMQ_ROUTER);
    clients.bind ("tcp://*:5555");
    zmq::socket_t workers (context, ZMQ_DEALER);
    workers.bind ("inproc://workers");

    // Launch pool of worker threads
    for (int thread_nbr = 0; thread_nbr != 5; thread_nbr++) {
        pthread_t worker;
        pthread_create (&worker, NULL, worker_routine, (void *) &context);
    }
    // Connect work threads to client threads via a queue
    zmq::proxy (static_cast<void*>(clients),
                static_cast<void*>(workers),
                nullptr);
    return 0;
}

