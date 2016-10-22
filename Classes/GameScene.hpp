//
//  GameScene.hpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "SpriteBird.hpp"
#include "SlingShot.hpp"

USING_NS_CC;

class Game : public Layer {
protected:
    Vector<SpriteBird*> _birdVector;  //小鸟集合
    SpriteBird* _currentBird;   //当前小鸟
    bool _isStart;  //游戏开始
    bool _isFinish; //游戏结束
    bool _isCollision;  //小鸟是否碰撞标记
    int _touchStatus;   //状态变量，是否触摸小鸟
    int _screenW,_screenH;
    DrawNode* drawNode; //drawNode用来完成一些绘图工作，直线 多边形等
    
    SlingShot* _slingShot;
    void jump();
    
    Rect rectOfSprite(Sprite* sprite);
    
    void playHitAnimation(float x, float y);
    
    bool hasPig();
    bool hasBird();
    
    
public:
    virtual bool init() override;
    static Scene* createSceneWithLevel(int selectLevel);
    void createLevel();
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags) override;
    
//    bool onContactBegin(PhysicsContact& contact);
//    bool onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve);
//    void onContactPostSolve(PhysicsContact& contact, const PhysicsContactPostSolve& solve);
//    void onContactSeperate(PhysicsContact& contact);
    
    void gameOver(std::string message);
//    void update(float delta);
    void update(float delta) override;
    
    CREATE_FUNC(Game);
};

#endif /* GameScene_hpp */
