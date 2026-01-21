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

    for (int i = static_cast<int>(Equipment::weapon); i < static_cast<int>(Equipment::count); i++){
        equipmentBonus[i].maxHealth = 0;
        equipmentBonus[i].attackDmg = 0;
        equipmentBonus[i].defense = 0;
        equipmentBonus[i].critChance = 0;
        equipmentBonus[i].critDmgMod = 0;
    };
} // End of Character initialization


int Character::getBaseStat(EnumStats wantedStat){
    switch (wantedStat){
        case EnumStats::maxHealth:
            return baseStats.maxHealth;

        case EnumStats::attackDmg:
            return baseStats.attackDmg;

        case EnumStats::defense:
            return baseStats.defense;

        case EnumStats::critChance:
            return baseStats.critChance;

        case EnumStats::critDmgMod:
            return baseStats.critDmgMod;
    }

    return 0;
} // End of getBaseStat method


int Character::getEquipStat(EnumStats wantedStat){

    int statVal = 0;

    switch (wantedStat){
        case EnumStats::maxHealth:
            for (int i = static_cast<int>(Equipment::weapon); i < static_cast<int>(Equipment::count); i++){
                statVal += equipmentBonus[i].maxHealth;
            };
            break;

        case EnumStats::attackDmg:
            for (int i = static_cast<int>(Equipment::weapon); i < static_cast<int>(Equipment::count); i++){
                statVal += equipmentBonus[i].attackDmg;
            };
            break;

        case EnumStats::defense:
            for (int i = static_cast<int>(Equipment::weapon); i < static_cast<int>(Equipment::count); i++){
                statVal += equipmentBonus[i].defense;
            };
            break;

        case EnumStats::critChance:
            for (int i = static_cast<int>(Equipment::weapon); i < static_cast<int>(Equipment::count); i++){
                statVal += equipmentBonus[i].critChance;
            };
            break;

        case EnumStats::critDmgMod:
            for (int i = static_cast<int>(Equipment::weapon); i < static_cast<int>(Equipment::count); i++){
                statVal += equipmentBonus[i].critDmgMod;
            };
            break;
    }

    return statVal;
} // End of getEquipStat method


int Character::getCurHealth(){
    return curHealth;
} // End of getCurHealth method


void Character::setBaseStat(EnumStats wantedStat, int val){
    switch (wantedStat){
        case EnumStats::maxHealth:
            baseStats.maxHealth = val;
            break;

        case EnumStats::attackDmg:
            baseStats.attackDmg = val;
            break;

        case EnumStats::defense:
            baseStats.defense = val;
            break;

        case EnumStats::critChance:
            baseStats.critChance = val;
            break;

        case EnumStats::critDmgMod:
            baseStats.critDmgMod = val;
            break;
    }

} // End of setBaseStat method


void Character::setEquipStat(EnumStats wantedStat, int val, Equipment item){
    switch (wantedStat){
        case EnumStats::maxHealth:
            equipmentBonus[static_cast<int>(item)].maxHealth = val;
            break;

        case EnumStats::attackDmg:
            equipmentBonus[static_cast<int>(item)].attackDmg = val;
            break;

        case EnumStats::defense:
            equipmentBonus[static_cast<int>(item)].defense = val;
            break;

        case EnumStats::critChance:
            equipmentBonus[static_cast<int>(item)].critChance = val;
            break;

        case EnumStats::critDmgMod:
            equipmentBonus[static_cast<int>(item)].critDmgMod = val;
            break;
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
        totalDmgTaken = dmgTaken * (100.00 / (100 + baseStats.defense + getEquipStat(EnumStats::defense)));
    }

    // Else
    //   Mulltipy total defense stat by 3
    else{
        totalDmgTaken = dmgTaken * (100.00 / (100 + 3 * (baseStats.defense + getEquipStat(EnumStats::defense))));
    }

    std::cout << "\ntook " << totalDmgTaken << " dmg\n";
    setCurHeatlh(curHealth - totalDmgTaken);
} // End of takeDmg method


int Character::calcAttackDmg(){
    // Regardless of the number generated,
    //  the character needs to have a critChance above 0 for crits to be available

    int dmgDealt = randomNumber() % (baseStats.attackDmg + getEquipStat(EnumStats::attackDmg));

    if (((randomNumber() % 100) < (baseStats.critChance + getEquipStat(EnumStats::critChance))) && ((baseStats.critChance + getEquipStat(EnumStats::critChance)) > 0)){
        critHit = true;
        std::cout << "- WOAH!!! NICE CRIT!!! - ";
    }

    if (critHit){
        dmgDealt = dmgDealt * (1.0 + (double)(baseStats.critDmgMod + getEquipStat(EnumStats::critDmgMod)) / 100);
    }

    return dmgDealt;
} // End of calcAttackDmg method


void Character::resetTurn(){
    defending = false;
    critHit = false;
} // End of resetTurn method
