// Character.h
#ifndef CHARACTER_H
#define CHARACTER_H
#include <string>

class Character{
    public:
        Character(int hp, int dmg, int def, int critC, int critDmg);
        struct stats {
            int maxHealth;
            int attackDmg;
            int defense;
            int critChance;
            int critDmgMod;
        };

        // Accessors
        int getBaseStat(std::string wantedStat);
        int getEquipStat(std::string wantedStat);
        int getCurHealth();

        // Modifiers
        void setBaseStat(std::string wantedStat, int val);
        void setEquipStat(std::string wantedStat, int val);
        void setCurHeatlh(int val);
        void setDefending(bool val);

        // Misc
        bool isAlive();
        void takeDmg(double dmgTaken);
        int calcAttackDmg();
        void resetTurn();

    private:
        stats baseStats;
        stats equipmentBonus;
        int curHealth;
        bool defending;
        bool critHit;


    }; // End of class declaration

#endif
