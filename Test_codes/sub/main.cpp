#include <iostream>
#include <zmq_addon.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

int main( void )
{
   try
   {
        string strKeuzeOpnieuwGooien = "1/2/X/4/x";

        // X eruit halen
        string arrOpnieuwGooienDobbelstenen[5];
        int arrGekozenDobbelstenen[5] = {9, 9, 9, 9, 9};
        stringstream string_stream(strKeuzeOpnieuwGooien);
        int lengteArray = (sizeof(arrOpnieuwGooienDobbelstenen) / sizeof(string));
        int index = 0;

        for (int i = 0; i < lengteArray; i++)
        {
            string substr;
            getline(string_stream, substr, '/');
            arrOpnieuwGooienDobbelstenen[i] = substr;
        }
        for (int i = 0; i < lengteArray; i++)
        {
            if (arrOpnieuwGooienDobbelstenen[i] == "X" || arrOpnieuwGooienDobbelstenen[i] == "x")
            {
                arrGekozenDobbelstenen[index] = i;
                index++;
            }
        }
        for (int i = 0; i < 5; i++)
        {
            if (arrGekozenDobbelstenen[i] != 9)
            {
                cout << arrGekozenDobbelstenen[i] << " ";
            }
        }
        cout << "zullen opnieuw gegooid worden" << endl;
   }
   catch( zmq::error_t & error )
   {
       cerr << "Error! : " << error.what();
   }
}
