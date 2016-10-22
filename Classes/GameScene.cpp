//
//  GameScene.cpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#include "GameScene.hpp"
#include "JsonParser.hpp"
#include "LevelUtils.hpp"
#include "SpritePig.hpp"
#include "SpriteIce.hpp"
#include "LevelScene.hpp"
#include "Tools.hpp"

#define SLINGSHOT_POS Vec2(170, 250)
#define TOUCH_UNKNOW 0
#define TOUCH_SELECT 1

static int _currentLevel = 1;

Scene* Game::createSceneWithLevel(int selectLevel){
    _currentLevel = selectLevel;
    //一个scene只有一个PhysicsWorld其下的所有layer共用一个PhysicsWorld实例
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0.0f, -200.0f));
    auto layer = Game::create();
    scene->addChild(layer);
    return scene;
}

bool Game::init(){
    if(!Layer::init())
        return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _screenW = visibleSize.width;
    _screenH = visibleSize.height;
    
    _isFinish = false;
    _isStart = false;
    _isCollision = false;
    
    //创建背景
    auto bgSprite = Sprite::create("bg.png");
    bgSprite->setPosition(_screenW/ 2, _screenH / 2);
    Tools::adapt(bgSprite, visibleSize);
    this->addChild(bgSprite);
    
    //创建一个节点
    auto wall = Node::create();
    
    //创建一个静态多边形刚体，表示屏幕四边
    Vec2 vers[4] = { Vec2(-visibleSize.width / 2, -160), Vec2(-visibleSize.width / 2, visibleSize.height / 2), Vec2(visibleSize.width / 2, visibleSize.height / 2), Vec2(visibleSize.width / 2, -160)};
    auto body = PhysicsBody::createEdgePolygon(vers, 4, PHYSICSBODY_MATERIAL_DEFAULT);
    body->setGroup(1);
    
    wall->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    wall->setPhysicsBody(body);
    this->addChild(wall);
    this->createLevel();
    
    auto leftShot = Sprite::create("leftshot.png");
    leftShot->setPosition(170, 220);
    this->addChild(leftShot);
    
    auto rightShot = Sprite::create("rightshot.png");
    rightShot->setPosition(170, 220);
    this->addChild(rightShot);
    
    _slingShot = SlingShot::create();
    _slingShot->getTexture()->setAntiAliasTexParameters();
    
    _slingShot->setStartPoint1(Vec2(164, 260));
    _slingShot->setStartPoint2(Vec2(184, 256));
    
    _slingShot->setEndPoint(SLINGSHOT_POS);
    _slingShot->setContentSize(Size(480, 320));
    _slingShot->setPosition(Point(240, 158));
    this->addChild(_slingShot, 10);
    
    drawNode = DrawNode::create();
    this->addChild(drawNode, 10);
    
    this->jump();
    
    auto gameListener = EventListenerTouchOneByOne::create();
    gameListener->onTouchBegan = [=](Touch* touch, Event* event){
        _touchStatus = TOUCH_UNKNOW;
        Vec2 touchLocation = touch->getLocation();
        Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
        if(_currentBird == nullptr)
            return false;
        
        Rect birdRect = this->rectOfSprite(_currentBird);
        if(_currentBird->isReady() && birdRect.containsPoint(nodeLocation)){
            _touchStatus = TOUCH_SELECT;
            return true;
        }
        return false;
    };
    
    gameListener->onTouchMoved = [=](Touch* touch, Event* event){
        if(_touchStatus == TOUCH_SELECT){
            Vec2 touchLocation = touch->getLocation();
            Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
            nodeLocation.x = MAX(SLINGSHOT_POS.x - 140, nodeLocation.x);
            nodeLocation.x = MIN(SLINGSHOT_POS.x + 140, nodeLocation.x);
            nodeLocation.y = MAX(SLINGSHOT_POS.y - 50, nodeLocation.y);
            nodeLocation.y = MIN(SLINGSHOT_POS.y + 140, nodeLocation.y);
            
            _slingShot->setEndPoint(nodeLocation);
            _currentBird->setPosition(nodeLocation);
        }
    };
    
    gameListener->onTouchEnded = [=](Touch* touch, Event* event){
        if(_touchStatus == TOUCH_SELECT){
            Vec2 touchLocation = touch->getLocation();
            Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
            _slingShot->setEndPoint(SLINGSHOT_POS);
            
            auto body = _currentBird->getPhysicsBody();
            body->setDynamic(true);
            float offX = nodeLocation.x - SLINGSHOT_POS.x;
            float offY = nodeLocation.y - SLINGSHOT_POS.y;
            float radian = atan2f(offY, offX);
            float endX = SLINGSHOT_POS.x - _screenW / 2 * cosf(radian) - SLINGSHOT_POS.x;
            float endY = SLINGSHOT_POS.y - _screenH / 2 * sinf(radian) - SLINGSHOT_POS.y;
            
            body->setVelocity(Vec2(endX, endY));
            
            _birdVector.eraseObject(_currentBird);
            auto callFunc = CallFunc::create([=]{
                this->jump();
                _isCollision = false;
            });
            auto action = Sequence::create(DelayTime::create(3), callFunc, NULL);
            this->runAction(action);
        }
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(gameListener, this);
    
    
    
    auto contactListener = EventListenerPhysicsContactWithGroup::create(0);
//    contactListener->onContactBegin = CC_CALLBACK_1(Game::onContactBegin, this);
//    contactListener->onContactPreSolve = CC_CALLBACK_2(Game::onContactPreSolve, this);
    contactListener->onContactPostSolve = [=](PhysicsContact& contact, const PhysicsContactPostSolve& solve){
            SpriteBase* spriteA = (SpriteBase*)contact.getShapeA()->getBody()->getNode();
            SpriteBase* spriteB = (SpriteBase*)contact.getShapeB()->getBody()->getNode();
            if(spriteA && spriteB){
                spriteA->setHP(spriteA->getHP() - 1);
                spriteB->setHP(spriteB->getHP() - 1);
                float x = 0, y = 0;
                if(spriteA->getTag() == BIRD_TAG){
                    x = spriteA->getPosition().x;
                    y = spriteA->getPosition().y;
                    this->playHitAnimation(x, y);
                }
        
                if(spriteB->getTag() == BIRD_TAG){
                    x = spriteB->getPosition().x;
                    y = spriteB->getPosition().y;
                    this->playHitAnimation(x, y);
                }
        
                if(spriteA->getHP() <= 0){
                    this->removeChild(spriteA);
                }
                
                if(spriteB->getHP() <= 0){
                    this->removeChild(spriteB);
                }
            }
    };
//    contactListener->onContactSeparate = CC_CALLBACK_1(Game::onContactSeperate, this);
    contactListener->onContactBegin = [=](PhysicsContact& contact){
        
        SpriteBase* spriteA = (SpriteBase*)contact.getShapeA()->getBody()->getNode();
        SpriteBase* spriteB = (SpriteBase*)contact.getShapeB()->getBody()->getNode();
        if(spriteA && spriteB){
            spriteA->setHP(spriteA->getHP() - 1);
            spriteB->setHP(spriteB->getHP() - 1);
            float x = 0, y = 0;
            if(spriteA->getTag() == BIRD_TAG){
                x = spriteA->getPosition().x;
                y = spriteA->getPosition().y;
                this->playHitAnimation(x, y);
            }
            
            if(spriteB->getTag() == BIRD_TAG){
                x = spriteB->getPosition().x;
                y = spriteB->getPosition().y;
                this->playHitAnimation(x, y);
            }
            
            if(spriteA->getHP() <= 0){
                this->removeChild(spriteA);
            }
            
            if(spriteB->getHP() <= 0){
                this->removeChild(spriteB);
            }
        }
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    this->scheduleUpdate();
    
    return true;
}

void Game::createLevel(){
    std::string fileName = StringUtils::format("%d.data", _currentLevel);
    JsonParser* parser = JsonParser::create();
    Vector<SpriteData*> spriteDataVector = parser->getAllSpriteWithFile(fileName);
    for(int i = 0; i < spriteDataVector.size();++i){
        auto spriteData = spriteDataVector.at(i);
        switch (spriteData->getTag()) {
            case PIG_TAG:
            {
                SpritePig* pig = SpritePig::create("pig1.png");
                pig->initWithData(spriteData->getX(), spriteData->getY());
                this->addChild(pig);
                break;
            }
                
            case ICE_TAG:
            {
                SpriteIce* ice = SpriteIce::create("ice1.png");
                ice->initWithData(spriteData->getX(), spriteData->getY());
                this->addChild(ice);
                break;
            }
                
            default:
                break;
        }
    }
    
    SpriteBird* bird = SpriteBird::create("bird1.png");
    bird->initWithData(320, 186);
    SpriteBird* bird2 = SpriteBird::create("bird1.png");
    bird2->initWithData(280, 186);
    SpriteBird* bird3 = SpriteBird::create("bird1.png");
    bird3->initWithData(240, 186);
    this->addChild(bird);
    this->addChild(bird2);
    this->addChild(bird3);
    
    _birdVector.pushBack(bird);
    _birdVector.pushBack(bird2);
    _birdVector.pushBack(bird3);
}

void Game::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags){
    drawNode->clear();
    drawNode->drawSegment(Vec2(_slingShot->getStartPoint1().x, _slingShot->getStartPoint1().y), Vec2(_slingShot->getEndPoint().x, _slingShot->getEndPoint().y), 1, Color4F(1, 0, 0, 1));
    drawNode->drawSegment(Vec2(_slingShot->getStartPoint2().x, _slingShot->getStartPoint2().y), Vec2(_slingShot->getEndPoint().x, _slingShot->getEndPoint().y), 1, Color4F(1, 0, 0, 1));
    
}

void Game::jump(){
    if(_birdVector.size() > 0 && !_isFinish){
        _currentBird = _birdVector.at(0);
        auto jumpTo = JumpTo::create(1, Vec2(170, 265), 50, 1);
        auto callFunc = CallFunc::create([=]{
            if(!_isFinish)
                _isStart = true;
            if(!_currentBird->isReady()){
                _currentBird->setReady(true);
            }
        });
        auto allActions = Sequence::create(jumpTo, callFunc, NULL);
        _currentBird->runAction(allActions);
    }
}

Rect Game::rectOfSprite(cocos2d::Sprite *sprite){
    return Rect(sprite->getPosition().x - sprite->getContentSize().width / 2, sprite->getPosition().y - sprite->getContentSize().height / 2, sprite->getContentSize().width + sprite->getContentSize().width / 2, sprite->getContentSize().height + sprite->getContentSize().height / 2);
}

//碰撞的方法

//void Game::onContactPostSolve(cocos2d::PhysicsContact &contact, const cocos2d::PhysicsContactPostSolve &solve){
//    SpriteBase* spriteA = (SpriteBase*)contact.getShapeA()->getBody()->getNode();
//    SpriteBase* spriteB = (SpriteBase*)contact.getShapeB()->getBody()->getNode();
//    if(spriteA && spriteB){
//        spriteA->setHP(spriteA->getHP() - 1);
//        spriteB->setHP(spriteB->getHP() - 1);
//        float x = 0, y = 0;
//        if(spriteA->getTag() == BIRD_TAG){
//            x = spriteA->getPosition().x;
//            y = spriteA->getPosition().y;
//            this->playHitAnimation(x, y);
//        }
//        
//        if(spriteB->getTag() == BIRD_TAG){
//            x = spriteB->getPosition().x;
//            y = spriteB->getPosition().y;
//            this->playHitAnimation(x, y);
//        }
//        
//        if(spriteA->getHP() <= 0){
//            this->removeChild(spriteA);
//        }
//        
//        if(spriteB->getHP() <= 0){
//            this->removeChild(spriteB);
//        }
//    }
//}

void Game::playHitAnimation(float x, float y){
    for(int i = 0; i < 6;++i){
        int range = 2;
        auto plume = Sprite::create("plume1.png");
        plume->setScale((float)(arc4random() % 5 / 10.1f));
        Vec2 pos = Vec2(x + arc4random() % 10 * range - 10, y + arc4random() % 10 * range - 10);
        plume->setPosition(pos);
        
        auto moveTo = MoveTo::create(1, pos);
        auto fadeOut = FadeOut::create(1);
        auto rotateBy = RotateBy::create(1, arc4random() % 180);
        auto spawn = Spawn::create(moveTo, fadeOut, rotateBy, NULL);
        plume->runAction(spawn);
        this->addChild(plume);
    }
}


bool Game::hasBird(){
    Vector<Node*> childrenVector = this->getChildren();
    for(int i = 0; i < childrenVector.size();++i){
        Node* node = childrenVector.at(i);
        if(node->getTag() == BIRD_TAG){
            return true;
        }
    }
    return false;
}

bool Game::hasPig(){
    Vector<Node*> childrenVector = this->getChildren();
    for(int i = 0; i < childrenVector.size();++i){
        Node* node = childrenVector.at(i);
        if(node->getTag() == PIG_TAG){
            return true;
        }
    }
    return false;
}

void Game::gameOver(std::string message){
    if(_currentLevel == LevelUtils::readLevelFromFile()){
        LevelUtils::writeLevelToFile(_currentLevel + 1);
    }
    this->unscheduleUpdate();
    
    auto finishSprite = Sprite::create("finish.png");
    finishSprite->setPosition(_screenW / 2, _screenH / 2);
    finishSprite->setScale(0.7);
    this->addChild(finishSprite);
    
    std::string levelString = StringUtils::format("第%d关", _currentLevel);
    auto levelLabel = Label::createWithSystemFont(levelString, "Marker Felt", 64);
    levelLabel->setColor(Color3B(255, 0, 0));
    levelLabel->setScale(0.6);
    levelLabel->setPosition(_screenW / 2, _screenH / 2 + 136);
    this->addChild(levelLabel);
    
    auto messageLabel = Label::createWithSystemFont(message, "Marker Felt", 128);
    messageLabel->setColor(Color3B(255, 0, 0));
    messageLabel->setScale(0.6);
    messageLabel->setPosition(_screenW / 2, _screenH / 2 - 50);
    this->addChild(messageLabel);
    
    auto selectItem = MenuItemImage::create("menu.png", "menu.png", [=](Ref* sender){
        auto transition = TransitionSplitRows::create(1.0f, Level::createScene());
        Director::getInstance()->replaceScene(transition);
    });
    
    selectItem->setScale(0.7);
    selectItem->setPosition(_screenW / 2 - 140, _screenH / 2 - 180);
    auto restartItem = MenuItemImage::create("restart.png", "restart.png", [=](Ref* sender){
        auto transition = TransitionSplitRows::create(1.0f, Game::createSceneWithLevel(_currentLevel));
        Director::getInstance()->replaceScene(transition);
    });
    restartItem->setScale(0.7f);
    restartItem->setPosition(_screenW / 2, _screenH / 2 - 180);
    
    auto nextItem = MenuItemImage::create("next.png", "next.png", [=](Ref* sender){
        auto transition = TransitionSplitRows::create(1.0f, Game::createSceneWithLevel(_currentLevel + 1));
        Director::getInstance()->replaceScene(transition);
    });
    nextItem->setScale(0.7f);
    nextItem->setPosition(_screenW / 2 + 140, _screenH / 2 - 180);
    
    auto menu = Menu::create(selectItem, restartItem, nextItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
}

void Game::update(float delta){
//    if(!_isCollision){
//        if(_currentBird->getPosition().x >= _screenW - 20 || _currentBird->getPosition().y >= _screenH - 20){
//            this->removeChild(_currentBird);
//        }
//    }
    
    if(!hasPig()){
        gameOver("过关");
    } else {
        if(!hasBird()){
            gameOver("再来一次");
        }
    }
}
