/* Within the ability to summon there is a chance to summon different monsters of different tiers which are higher quality
Items will be picked up which can add an increase chance for a higher quality tier
Items with "Quality Chance" (QC) stats must be first be equipped for other roles to be 
    Otherwise only default summons will occur
The chance for each tier will be manipulated based on the total QC stats from equipment:
    - Tiers are locked behind a threshold
        A specific QC must be reached before that tier is available
    - As QC increase, the chance of lower tier summons decrease
    - 5:3:2 ratio for the chance of B, A, and S tier respectively when all are available
*/

#include <iostream>
#include <ctime>
#include <typeinfo>

using std::cout;
using std::cin;
using std::getline;
using std::string;

bool qcEquip = false;

void sumQuality(int qualStat);

int main(){

    srand(time(0));
    int qualityStats;
    
    cout << "Sum Qual: ";
    cin >> qualityStats;
    
    if (qualityStats > 0){
        qcEquip = true;
    }

    if(qcEquip){
        sumQuality(qualityStats);
    }
    else{
        cout << "Nothing Equipped";
    }

    return 0;
}

void sumQuality(int qualStat){
    
    int sumQual = (rand() % 100);

    double bTierChan = 100 - qualStat;
    double aTierChan = bTierChan + (0.5 * qualStat);
    double sTierChan = aTierChan + (0.3 * qualStat);

    /* Summonn Tier Threshold
    B tier: QC <= 0
    A tier: QC <= 30
    S tier: QC <= 50
    */

    if(qualStat >= 50 && sumQual >= sTierChan){
        cout << " S Tier: SUMMONED!\n";
    }
    else if(qualStat >= 30 && sumQual >= aTierChan){
        cout << " A Tier: SUMMONED!\n";
    }
    else if(sumQual >= bTierChan){
        cout << " B Tier: SUMMONED!\n";
    }
    else{
        cout << "wow... a default..\n";
    }

} // end of sumQuality