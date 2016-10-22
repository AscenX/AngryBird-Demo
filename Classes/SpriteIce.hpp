//
//  SpriteIce.hpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#ifndef SpriteIce_hpp
#define SpriteIce_hpp

#include "SpriteBase.hpp"

class SpriteIce : public SpriteBase {
public:
    static SpriteIce* create(const std::string& filename){
        SpriteIce* ice = new SpriteIce();
        if(ice && ice->initWithFile(filename)){
            ice->autorelease();
            return ice;
        }
        CC_SAFE_DELETE(ice);
        return nullptr;
    }
    
    void initWithData(float x, float y);
};

#endif
