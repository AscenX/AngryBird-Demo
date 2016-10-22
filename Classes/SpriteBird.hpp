//
//  SpriteBird.hpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#ifndef SpriteBird_hpp
#define SpriteBird_hpp

#include "SpriteBase.hpp"

class SpriteBird : public SpriteBase {
protected:
    bool _isReady;
    bool _isFly;
public:
    static SpriteBird* create(const std::string& filename){
        SpriteBird* bird = new SpriteBird();
        if(bird && bird->initWithFile(filename)){
            bird->autorelease();
            return bird;
        }
        CC_SAFE_DELETE(bird);
        return nullptr;
    }
    
    void initWithData(float x, float y);
    
    void setReady(bool isReady){ _isReady = isReady;};
    bool isReady(){ return _isReady;};
    void setFly(bool isFly){ _isFly = isFly;};
    bool isFly(){ return _isFly;};
};

#endif /* SpriteBird_hpp */
