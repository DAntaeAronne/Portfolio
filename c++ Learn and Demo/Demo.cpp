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
    string statsNames[5] = {"maxHealth", "attackDmg", "defense", "critChance", "critDmgMod"};

    Character MC(150, 250, 6, 0, 900);
    for (int i = 0; i < static_cast<int>(Character::EnumStats::count); i++){

        Character::EnumStats statCheck = static_cast<Character::EnumStats>(i);
        cout << statsNames[i] << ": " << MC.getBaseStat(statCheck) << "\n";
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

    cout << "\n\n\n\n\n";
    cout << "test: \n";
    cout << "max health: " << MC.getBaseStat(Character::EnumStats::maxHealth) << "\n\n";
    cout << "cur health: " << MC.getCurHealth();

    return 0;
}
