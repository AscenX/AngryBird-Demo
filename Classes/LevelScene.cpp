//
//  LevelScene.cpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#include "LevelScene.hpp"
#include "LevelUtils.hpp"
#include "StartScene.hpp"
#include "GameScene.hpp"
#include "Tools.hpp"



Scene* Level::createScene(){
    auto scene = Scene::create();
    auto layer = Level::create();
    scene->addChild(layer);
    return scene;
}

bool Level::init(){
    if(!Layer::init())
        return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _screenW = visibleSize.width;
    _screenH = visibleSize.height;
    
    auto selectLevel = Sprite::create("selectlevel.png");
    selectLevel->setPosition(_screenW / 2, _screenH / 2);
    Tools::adapt(selectLevel, visibleSize);
    this->addChild(selectLevel);
    
    auto back = Sprite::create("back.png");
    back->setPosition(40, 40);
    back->setScale(0.8f);
    
    back->setTag(100);
    this->addChild(back);
    
    _successLevel = LevelUtils::readLevelFromFile();
    
    std::string imagePath = "";
    for(int i = 0;i < 12;++i){
        if(i < _successLevel){
            imagePath = "level.png";
            std::string str = StringUtils::format("%d", i + 1);
            auto *num = Label::createWithSystemFont(str, "Arial-BoldMT", 40);
            float x = 100 + i % 6 * (_screenW / 6 - 15);
            float y = _screenH - 190 - i / 6 * 150;
            num->setPosition(x, y);
            this->addChild(num, 2);
        } else {
            imagePath = "clock.png";
        }
        
        auto level = Sprite::create(imagePath);
        level->setTag(i+1);
        
        float x = 100 + i % 6 * (_screenW / 6 - 15);
        float y = _screenH - 180 - i / 6 * 150;
        level->setPosition(x, y);
        level->setScale(0.7f);
        this->addChild(level, 1);
    }
    
    auto gameListener = EventListenerTouchOneByOne::create();
    gameListener->onTouchBegan = ([=](Touch* touch, Event* event){
        Vec2 touchLocation = touch->getLocation();
        Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
        for(int i = 0; i < this->getChildrenCount();++i){
            auto tempSprite = this->getChildren().at(i);
            if(tempSprite->getBoundingBox().containsPoint(nodeLocation)){
                if(tempSprite->getTag() == 100){
                    auto transition = TransitionSplitRows::create(2.0f, Start::createScene());
                    Director::getInstance()->replaceScene(transition);
                } else if((tempSprite->getTag() < _successLevel + 1) && tempSprite->getTag() > 0){
                    Director::getInstance()->getEventDispatcher()->removeEventListener(gameListener);
                    auto transition = TransitionSplitRows::create(2.0f, Game::createSceneWithLevel(tempSprite->getTag()));
                    Director::getInstance()->replaceScene(transition);
                } else if(tempSprite->getTag() < 13 && tempSprite->getTag() > 1){
                    std::string message = StringUtils::format("第[%d]关暂时未开放", tempSprite->getTag());
                    MessageBox(message.c_str(), "提示");
                }
            }
        }
        return true;
    });
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(gameListener, this);
    
    return true;
}
