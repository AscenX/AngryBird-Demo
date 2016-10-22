//
//  LevelScene.hpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#ifndef LevelScene_hpp
#define LevelScene_hpp

#include "cocos2d.h"

USING_NS_CC;

class Level : public Layer {
private:
    int _successLevel = 1;
    int _screenW,_screenH;
public:
    static Scene* createScene();
    virtual bool init();
    
    CREATE_FUNC(Level);
};

#endif /* LevelScene_hpp */
