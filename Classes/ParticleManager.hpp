//
//  ParticleManager.hpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#ifndef ParticleManager_hpp
#define ParticleManager_hpp

#include "cocos2d.h"

USING_NS_CC;

typedef enum {
    snow,
    explosition
} ParticleTypes;

class ParticleManager : public Ref {
public:
    static ParticleManager* getInstance();
    ParticleSystem* particleWithType(ParticleTypes type);
};

#endif /* ParticleManager_hpp */
