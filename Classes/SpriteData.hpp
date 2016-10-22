//
//  SpriteData.hpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#ifndef SpriteData_hpp
#define SpriteData_hpp

#include "cocos2d.h"

USING_NS_CC;

class SpriteData : public Ref {
protected:
    int _tag;
    int _x,_y;
public:
    static SpriteData* create(){
        SpriteData* sprite = new SpriteData();
        if(sprite){
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    
    void setTag(int tag) { _tag = tag;};
    int getTag(){ return _tag;};
    void setX(int x){_x = x;};
    int getX(){ return _x;};
    void setY(int y){_y = y;};
    int getY(){ return _y;};
};

#endif /* SpriteData_hpp */
