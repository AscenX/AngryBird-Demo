//
//  ParticleManager.cpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#include "ParticleManager.hpp"

static ParticleManager* sharedParticleManager = nullptr;

ParticleManager* ParticleManager::getInstance(){
    if(!sharedParticleManager)
        sharedParticleManager = new ParticleManager();
    return sharedParticleManager;
}

ParticleSystem* ParticleManager::particleWithType(ParticleTypes type){
    ParticleSystem* particleSystem = nullptr;
    switch (type) {
        case snow:
        {
            particleSystem = ParticleSnow::create();
            Texture2D* texture2D = Director::getInstance()->getTextureCache()->addImage("snow.png");
            particleSystem->setTexture(texture2D);
            break;
        }
            
        case explosition:
        {
            particleSystem = ParticleSystemQuad::create("bird-explosition.plist");
            particleSystem->setPositionType(ParticleSystem::PositionType::FREE);
            particleSystem->setAutoRemoveOnFinish(true);
            break;
        }
        default:
            break;
    }
    return particleSystem;
}


