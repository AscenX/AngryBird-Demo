//
//  LevelUtils.cpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#include "LevelUtils.hpp"

int LevelUtils::readLevelFromFile(){
    int successLevel = UserDefault::getInstance()->getIntegerForKey("SuccessLevel");
    if(successLevel){
        return successLevel;
    }
    return 1;
}

void LevelUtils::writeLevelToFile(int level){
    UserDefault::getInstance()->setIntegerForKey("SuccessLevel", level);
}
