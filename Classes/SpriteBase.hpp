//
//  SpriteBase.hpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#ifndef SpriteBase_hpp
#define SpriteBase_hpp

#include "cocos2d.h"

USING_NS_CC;

#define BIRD_TAG 1
#define PIG_TAG 2
#define ICE_TAG 3
#define WALL_TAG 4

class SpriteBase : public Sprite {
protected:
    float _HP;
    std::string _imageName;
public:
    static SpriteBase* create(const std::string& filename){
        SpriteBase* sprite = new SpriteBase();
        if(sprite && sprite->initWithFile(filename)){
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    
    void setImageName(std::string imageName){ _imageName = imageName;};
    std::string getImageName(){ return _imageName;};
    void setHP(float hp){ _HP = hp;};
    float getHP(){ return _HP;};
};

#endif /* SpriteBase_hpp */
