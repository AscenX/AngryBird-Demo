//
//  SpritePig.cpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#include "SpritePig.hpp"

void SpritePig::initWithData(float x, float y){
    this->setImageName("pig");
    this->setTag(PIG_TAG);
    this->setPosition(x, y);
    this->setHP(1);
    this->setScale(2.0f / 10);
    
    auto body = PhysicsBody::createEdgeBox(this->getContentSize() / 2);
    body->setDynamic(false);
    body->setContactTestBitmask(0x01);
    body->setCollisionBitmask(0x01);
    body->setGroup(0);
    this->setPhysicsBody(body);
}
