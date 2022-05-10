#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main (void)
{
    printf ("Connecting to hello world server...\n");
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

    while (true) {
        char buffer [10];
        printf ("Sending Hello\n");
        zmq_send (requester, "Hello", 20, 0);
        zmq_recv (requester, buffer, 20, 0);
        printf ("Received World\n");
        sleep(1);

        printf ("Sending GIJS\n");
        zmq_send (requester, "GIJS", 20, 0);
        zmq_recv (requester, buffer, 20, 0);
        printf ("Received JACKERS\n");
        sleep(1);
    }

    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
