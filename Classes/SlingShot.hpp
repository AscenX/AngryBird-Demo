//
//  SlingShot.hpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#ifndef SlingShot_hpp
#define SlingShot_hpp

#include "cocos2d.h"

USING_NS_CC;

class SlingShot : public Sprite {
protected:
    Vec2 _startPoint1;
    Vec2 _startPoint2;
    Vec2 _endPoint;
public:
    static SlingShot* create(){
        SlingShot* sprite = new SlingShot();
        if(sprite && sprite->init()){
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    
    void setStartPoint1(Vec2 startPoint1){ _startPoint1 = startPoint1;};
    Vec2 getStartPoint1(){ return _startPoint1;};
    void setStartPoint2(Vec2 startPoint2){ _startPoint2 = startPoint2;};
    Vec2 getStartPoint2(){ return _startPoint2;};
    void setEndPoint(Vec2 endPoint){ _endPoint = endPoint;};
    Vec2 getEndPoint(){ return _endPoint;};
};

#endif /* SlingShot_hpp */
