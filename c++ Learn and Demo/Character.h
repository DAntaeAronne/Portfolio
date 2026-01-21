// Character.h
#ifndef CHARACTER_H
#define CHARACTER_H
#include <string>

class Character{
    public:
        Character(int hp, int dmg, int def, int critC, int critDmg);
        struct Stats {
            int maxHealth;
            int attackDmg;
            int defense;
            int critChance;
            int critDmgMod;
        };

        enum struct EnumStats{
            maxHealth,
            attackDmg,
            defense,
            critChance,
            critDmgMod,
            count,
        };

        enum struct Equipment{
            weapon,
            armor,
            helmet,
            count,
        };

        // Accessors
        int getBaseStat(EnumStats wantedStat);
        int getEquipStat(EnumStats wantedStat);
        int getCurHealth();

        // Modifiers
        void setBaseStat(EnumStats wantedStat, int val);
        void setEquipStat(EnumStats wantedStat, int val, Equipment item);
        void setCurHeatlh(int val);
        void setDefending(bool val);

        // Misc
        bool isAlive();
        void takeDmg(double dmgTaken);
        int calcAttackDmg();
        void resetTurn();

    private:
        Stats baseStats;
        Stats equipmentBonus[3];
        int curHealth;
        bool defending;
        bool critHit;


    }; // End of class declaration

#endif
