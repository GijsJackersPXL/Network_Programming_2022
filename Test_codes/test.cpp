#include <iostream>
#include <vector>

// using namespace std;
int puntenSpeler[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int somSpeler = 0;
int bonusSpeler = 0;
int totaalPuntenSpeler = 0;
std::string naamSpeler1[] = {"g", "i", "j", "s"};

std::string arrPrintTabel[35];

int main()
{
    somSpeler = 0;
    bonusSpeler = 0;
    totaalPuntenSpeler = 0;

    for (int i = 0; i <= 5; i++)
    {
        somSpeler += puntenSpeler[i];
    }

    if (somSpeler >= 63)
    {
        bonusSpeler = 35;
    }

    for (int i = 6; i < 13; i++)
    {
        totaalPuntenSpeler += puntenSpeler[i];
    }
    totaalPuntenSpeler += bonusSpeler + somSpeler;

    arrPrintTabel[0] = " --------------------";
    arrPrintTabel[1] = "| Speler         | " + naamSpeler1[0] + " |";
    arrPrintTabel[2] = " --------------------";
    arrPrintTabel[3] = "| Enen           | " + std::to_string(puntenSpeler[0]) + " |";
    arrPrintTabel[4] = " --------------------";
    arrPrintTabel[5] = "| Tweeen         | " + std::to_string(puntenSpeler[1]) + " |";
    arrPrintTabel[6] = " --------------------";
    arrPrintTabel[7] = "| Drieen         | " + std::to_string(puntenSpeler[2]) + " |";
    arrPrintTabel[8] = " --------------------";
    arrPrintTabel[9] = "| Vieren         | " + std::to_string(puntenSpeler[3]) + " |";
    arrPrintTabel[10] = " --------------------";
    arrPrintTabel[11] = "| Vijven         | " + std::to_string(puntenSpeler[4]) + " |";
    arrPrintTabel[12] = " --------------------";
    arrPrintTabel[13] = "| Zessen         | " + std::to_string(puntenSpeler[5]) + " |";
    arrPrintTabel[14] = " --------------------";
    arrPrintTabel[15] = "| SOM            | " + std::to_string(somSpeler) + " |";
    arrPrintTabel[16] = " --------------------";
    arrPrintTabel[17] = "| BONUS          | " + std::to_string(bonusSpeler) + " |";
    arrPrintTabel[18] = " --------------------";
    arrPrintTabel[19] = "| Drie gelijke   | " + std::to_string(puntenSpeler[6]) + " |";
    arrPrintTabel[20] = " --------------------";
    arrPrintTabel[21] = "| Vier gelijke   | " + std::to_string(puntenSpeler[7]) + " |";
    arrPrintTabel[22] = " --------------------";
    arrPrintTabel[23] = "| Kleine straat  | " + std::to_string(puntenSpeler[8]) + " |";
    arrPrintTabel[24] = " --------------------";
    arrPrintTabel[25] = "| Grote straat   | " + std::to_string(puntenSpeler[9]) + " |";
    arrPrintTabel[26] = " --------------------";
    arrPrintTabel[27] = "| Full House     | " + std::to_string(puntenSpeler[10]) + " |";
    arrPrintTabel[28] = " --------------------";
    arrPrintTabel[29] = "| Kans           | " + std::to_string(puntenSpeler[11]) + " |";
    arrPrintTabel[30] = " --------------------";
    arrPrintTabel[31] = "| Yahtzee        | " + std::to_string(puntenSpeler[12]) + " |";
    arrPrintTabel[32] = " --------------------";
    arrPrintTabel[33] = "| TOTAAL         | " + std::to_string(totaalPuntenSpeler) + " |";
    arrPrintTabel[34] = " --------------------";

    for (int i = 0; i < 35; i++)
    {
        std::cout << arrPrintTabel[i] << std::endl;
    }
}