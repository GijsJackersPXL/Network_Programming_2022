#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int gooiDobbelsteen();

int main()
{
    srand(time(0)); 

    char gegooideDobbelstenen[6];

    for (int i = 0; i < 5; i++)
    {
        gegooideDobbelstenen[i] = gooiDobbelsteen() + '0';
    }
    for (int i = 0; i < 5; i++)
    {
        cout << i << ": " << gegooideDobbelstenen[i] << endl;
    }

    string sendTopic("GijsJackers>Yahtzee?>Dobbelstenen");
    for (int i = 0; i < 5; i++)
    {
        sendTopic += gegooideDobbelstenen[i];
    } 
    cout << sendTopic << endl;
}

int gooiDobbelsteen()
{
    return (rand() % 6) + 1;
}
