// Character.cpp
//
// Array of equipement
//  When new equipment is acquired then all of the stats need to be recombined
#include "Character.h"
#include "RNG.h"
#include <iostream>
#include <random>


using std::string;

Character::Character(int maxHp, int dmg, int def, int critC, int critDmg){
    baseStats.maxHealth = maxHp;
    baseStats.attackDmg = dmg;
    baseStats.defense = def;
    baseStats.critChance = critC;
    baseStats.critDmgMod = critDmg;
    curHealth = baseStats.maxHealth;
    defending = false;
    critHit = false;

    equipmentBonus.maxHealth = 0;
    equipmentBonus.attackDmg = 0;
    equipmentBonus.defense = 0;
    equipmentBonus.critChance = 0;
    equipmentBonus.critDmgMod = 0;
}

int Character::getBaseStat(string wantedStat){
    if (wantedStat == "maxHp"){
        return baseStats.maxHealth;
    }
    else if (wantedStat == "dmg"){
        return baseStats.attackDmg;
    }
    else if (wantedStat == "def"){
        return baseStats.defense;
    }
    else if (wantedStat == "critC"){
        return baseStats.critChance;
    }
    else if (wantedStat ==  "critDmg"){
        return baseStats.critDmgMod;
    }

    return 0;
} // End of getBaseStat method


int Character::getEquipStat(string wantedStat){
    if (wantedStat == "maxHp"){
        return equipmentBonus.maxHealth;
    }
    else if (wantedStat == "dmg"){
        return equipmentBonus.attackDmg;
    }
    else if (wantedStat == "def"){
        return equipmentBonus.defense;
    }
    else if (wantedStat == "critC"){
        return equipmentBonus.critChance;
    }
    else if (wantedStat ==  "critDmg"){
        return equipmentBonus.critDmgMod;
    }

    return 0;
} // End of getEquipStat method


int Character::getCurHealth(){
    return curHealth;
} // End of getCurHealth method


void setBaseStat(std::string wantedStat, int val){
    if (wantedStat == "maxHp"){
        baseStats.maxHealth = val;
    }
    else if (wantedStat == "dmg"){
        baseStats.attackDmg = val;
    }
    else if (wantedStat == "def"){
        baseStats.defense = val;
    }
    else if (wantedStat == "critC"){
        baseStats.critChance = val;
    }
    else if (wantedStat ==  "critDmg"){
        baseStats.critDmgMod = val;
    }
} // End of setBaseStat method


void setEquipStat(std::string wantedStat, int val){
    if (wantedStat == "maxHp"){
        equipmentBonus.maxHealth = val;
    }
    else if (wantedStat == "dmg"){
        equipmentBonus.attackDmg = val;
    }
    else if (wantedStat == "def"){
        equipmentBonus.defense = val;
    }
    else if (wantedStat == "critC"){
        equipmentBonus.critChance = val;
    }
    else if (wantedStat ==  "critDmg"){
        equipmentBonus.critDmgMod = val;
    }
} // End of setEquipStat method


void Character::setCurHeatlh(int val){
    curHealth = val;
} // End of setCurHeatlh method


void Character::setDefending(bool val){
    defending = val;
} // End of setDefending method


bool Character::isAlive(){
    return (curHealth > 0 ? true : false);
} // End of setEquipStat method


void Character::takeDmg(double dmgTaken){
    int totalDmgTaken;

    // If not defending
    //   Then take damage with only passive defense in mind
    if (!defending){
        totalDmgTaken = dmgTaken * (100.00 / (100 + baseStats.defense + equipmentBonus.defense));
    }

    // Else
    //   Mulltipy total defense stat by 3
    else{
        totalDmgTaken = dmgTaken * (100.00 / (100 + 3 * (baseStats.defense + equipmentBonus.defense)));
    }

    std::cout << "\ntook " << totalDmgTaken << " dmg\n";
    setCurHeatlh(curHealth - totalDmgTaken);
} // End of takeDmg method


int Character::calcAttackDmg(){
    // Regardless of the number generated,
    //  the character needs to have a critChance above 0 for crits to be available

    int dmgDealt = randomNumber() % (baseStats.attackDmg + equipmentBonus.attackDmg);

    if (((randomNumber() % 100) < (baseStats.critChance + equipmentBonus.critChance)) && ((baseStats.critChance + equipmentBonus.critChance) > 0)){
        critHit = true;
        std::cout << "- WOAH!!! NICE CRIT!!! - ";
    }

    if (critHit){
        dmgDealt = dmgDealt * (1.0 + (double)(baseStats.critDmgMod + equipmentBonus.critDmgMod) / 100);
    }

    return dmgDealt;
} // End of calcAttackDmg method


void Character::resetTurn(){
    defending = false;
    critHit = false;
} // End of resetTurn method
