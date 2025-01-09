//
// Created by hous on 12/25/24.
//

#include "Leveling.h"

#include <algorithm>

bool Leveling::checkXP(int XP) {
    for (Level& level : Levels ) {
        if (XP>= level.xpNeeded) {
            Levels.erase(std::remove_if(Levels.begin(), Levels.end(),
           [XP](const Level& level) { return level.xpNeeded<=XP; }), Levels.end());
            return true;
            //NOTE : XP  est dit dans la 'capture list' de la lambda
        }
    }
    return false;
}
