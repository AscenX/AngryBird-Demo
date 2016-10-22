
//
//  SpriteIce.cpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#include "SpriteIce.hpp"


void SpriteIce::initWithData(float x, float y){
    this->setImageName("ice");
    this->setTag(ICE_TAG);
    this->setPosition(x, y);
    this->setHP(10);
    this->setScale(2.0f / 10);
    
    auto body = PhysicsBody::createEdgeBox(this->getContentSize() / 5.5);
    body->setContactTestBitmask(0x01);
    body->setCollisionBitmask(0x01);
    body->setGroup(2);
    this->setPhysicsBody(body);
}
