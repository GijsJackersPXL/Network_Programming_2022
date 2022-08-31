#include <QCoreApplication>
#include "client_yahtzee.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client *c1 = new Client();
    c1->startClient();

    return a.exec();
}
