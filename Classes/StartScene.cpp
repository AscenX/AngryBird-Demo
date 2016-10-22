//
//  StartScene.cpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#include "StartScene.hpp"
#include "ParticleManager.hpp"
#include "LevelScene.hpp"
#include "Tools.hpp"

Scene* Start::createScene(){
    auto scene = Scene::create();
    auto layer = Start::create();
    scene->addChild(layer);
    return scene;
}

bool Start::init(){
    if(!Layer::init())
        return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto startbg = Sprite::create("startbg.png");
    startbg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    Tools::adapt(startbg, visibleSize);
    this->addChild(startbg);
    
    auto angryBird = Sprite::create("angrybird.png");
    Tools::adapt(angryBird, Size(800, 140));
    angryBird->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(angryBird);
    
    auto start = Sprite::create("start.png");
    Tools::adapt(start, Size(316, 210));
    auto startMenuItem = MenuItemSprite::create(start, start, CC_CALLBACK_0(Start::startGame, this));
    
    auto menu = Menu::create(startMenuItem, NULL);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    Tools::adapt(menu, visibleSize);
    this->addChild(menu, 1);
    
    auto particleSnow = ParticleManager::getInstance()->particleWithType(snow);
    this->addChild(particleSnow);
    
    this->schedule(schedule_selector(Start::birdJump), 1);
    
    return true;
}

void Start::startGame(){
    auto transition = TransitionSplitRows::create(2.0f, Level::createScene());
    Director::getInstance()->replaceScene(transition);
}

void Start::birdJump(float delta){
    this->createJumpBird();
}

void Start::createJumpBird(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto bird = Sprite::create("bird1.png");
    bird->setScale((arc4random() % 5) / 10.0f);
    bird->setPosition(50.0f + arc4random() % 50, 70.0f);
    Vec2 endPoint = Vec2(visibleSize.width * 0.8f + arc4random() % 50, 65.0f);
    
    float height = arc4random() % 100 + 50.0f;
    auto actionJump = JumpTo::create(2.0f, endPoint, height, 1);
    
    auto callFuncN = CallFuncN::create(CC_CALLBACK_0(Start::birdExplosition, this, bird));
    auto allActions = Sequence::create(actionJump, callFuncN, NULL);
    bird->runAction(allActions);
    this->addChild(bird);
}

void Start::birdExplosition(cocos2d::Ref *pSender){
    Node* node = static_cast<Node*>(pSender);
    auto particleExplosition = ParticleManager::getInstance()->particleWithType(explosition);
    particleExplosition->setPosition(node->getPosition());
    this->addChild(particleExplosition);
    this->removeChild(node);
}
