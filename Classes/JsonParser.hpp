//
//  JsonParser.hpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#ifndef JsonParser_hpp
#define JsonParser_hpp

#include "cocos2d.h"
#include "SpriteData.hpp"

USING_NS_CC;

class JsonParser : public Ref {
public:
    static JsonParser* create(){
        JsonParser* sprite = new JsonParser();
        if(sprite){
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    Vector<SpriteData*> getAllSpriteWithFile(std::string fileName);
};

#endif /* JsonParser_hpp */
