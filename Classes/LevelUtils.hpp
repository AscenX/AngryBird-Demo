//
//  LevelUtils.hpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#ifndef LevelUtils_hpp
#define LevelUtils_hpp

#include "cocos2d.h"

USING_NS_CC;

class LevelUtils : public Ref {
public:
    static int readLevelFromFile();
    static void writeLevelToFile(int level);
};

#endif /* LevelUtils_hpp */
