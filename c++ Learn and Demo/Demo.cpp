// Demo.cpp
#include <iostream>
#include "Character.h"
#include "Combat.h"
#include <ctime>

using std::cout;
using std::cin;
using std::getline;
using std::string;

int main(){

    srand(time(0));
    int choice;
    string statsNames[5] = {"maxHp", "dmg", "def", "critC", "critDmg"};

    Character MC(150, 250, 6, 0, 900);
    for (int i = 0; i < 5; i++){
        cout << statsNames[i] << ": " << MC.getBaseStat(statsNames[i]) << "\n";
    }

    cout << "\n\n";
    cout << "Are we alive?\n";

    if (MC.isAlive()){
        cout << "yes";
    }
    else {
        cout << "dead as dirt";
        return 0;
    }

    cout << "\n";
    cout << "Seems you have " << MC.getCurHealth() << " hp left..\n";
    cout << "catch *throws knife*\n\n";

    MC.takeDmg(24);

    cout << "How are you now?\n";
    cout << "Seems you have " << MC.getCurHealth() << " hp left..\n";
    cout << "catch *throws knife*\n\n";


    MC.setDefending(true);
    MC.takeDmg(MC.getCurHealth() + 10);

    cout << "Well, still around?\n";
    if (MC.isAlive()){
        cout << "yes";
    }
    else {
        cout << "dead as dirt";
        return 0;
    }

    cout << "\n";
    cout << "HOW?!?!\n";
    cout << "With " << MC.getCurHealth() << " hp left?!?!\n";
    cout << "Cheater\n";

    cout << "W-Wait!! You can't attack me ba- *pow*\n";

    for (int i = 0; i < 10; i++){
        cout << "(you did " << MC.calcAttackDmg() << " dmg. Nice)\n";
    }

    return 0;
}
