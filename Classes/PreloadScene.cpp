//
//  PreloadScene.cpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#include "PreloadScene.hpp"
#include "StartScene.hpp"
#include "Tools.hpp"

void Preload::load(float delta){
    auto ac = ProgressTo::create(1, 100 - _progressInterval * _sourceCount);
    _progress->runAction(ac);
    if(_sourceCount < 0){
        auto scene = Start::createScene();
        Director::getInstance()->replaceScene(scene);
        log("所有资源加载完毕");
        _progress->stopAllActions();
    }
    --_sourceCount;
}

Scene* Preload::createScene(){
    auto scene = Scene::create();
    auto layer = Preload::create();
    scene->addChild(layer);
    return scene;
}

bool Preload::init(){
    if(!Layer::init())
        return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto loading = Sprite::create("loading.png");
    loading->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    Tools::adapt(loading, visibleSize);
    this->addChild(loading);
    
    auto bar = Sprite::create("progressbar.png");
    _progress = ProgressTimer::create(bar);
    _progress->setPercentage(0.0f);
    _progress->setScale(0.5f);
    _progress->setMidpoint(Vec2(0, 0.5));
    _progress->setBarChangeRate(Vec2(1, 0));
    _progress->setType(ProgressTimer::Type::BAR);
    _progress->setPosition(visibleSize.width / 2 - 50, 30);
//    Tools::adapt(bar, Size(visibleSize.width * 0.9f, 160));
    this->addChild(_progress);
    
    _sourceCount = 2;   //模拟需要加载的资源数
    
    _progressInterval = 100.0 / static_cast<float>(_sourceCount);
    this->schedule(schedule_selector(Preload::load), 1.0f);
    
    return true;
}

