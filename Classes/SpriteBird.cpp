
//
//  SpriteBird.cpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#include "SpriteBird.hpp"

void SpriteBird::initWithData(float x, float y){
    this->setImageName("bird");
    this->setTag(BIRD_TAG);
    this->setPosition(x, y);
    this->setHP(1);
    this->setScale(0.3f);
    
    //添加物理引擎
    auto body = PhysicsBody::createCircle(this->getContentSize().width / 3 - 8);
    body->setDynamic(false);
    body->setCategoryBitmask(0x01);
    body->setContactTestBitmask(0x01);
    body->setCollisionBitmask(0x01);
    body->setGroup(0);
    this->setPhysicsBody(body);
}
