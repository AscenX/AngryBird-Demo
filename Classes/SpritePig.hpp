//
//  SpritePig.hpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#ifndef SpritePig_hpp
#define SpritePig_hpp

#include "SpriteBase.hpp"

class SpritePig : public SpriteBase {
public:
    static SpritePig* create(const std::string& filename){
        SpritePig* pig = new SpritePig();
        if(pig && pig->initWithFile(filename)){
            pig->autorelease();
            return pig;
        }
        CC_SAFE_DELETE(pig);
        return nullptr;
    }
    
    void initWithData(float x, float y);
};

#endif /* SpritePig_hpp */
