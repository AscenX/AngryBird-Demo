//
//  StartScene.hpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#ifndef StartScene_hpp
#define StartScene_hpp

#include "cocos2d.h"

USING_NS_CC;

class Start : public Layer {
public:
    static Scene* createScene();
    virtual bool init();
    void startGame();
    
    void birdJump(float delta);
    void createJumpBird();
    void birdExplosition(Ref* pSender);
    
    CREATE_FUNC(Start);
};

#endif /* StartScene_hpp */
