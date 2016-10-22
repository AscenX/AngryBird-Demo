//
//  PreloadScene.hpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#ifndef PreloadScene_hpp
#define PreloadScene_hpp

#include "cocos2d.h"

USING_NS_CC;

class Preload : public Layer {
private:
    ProgressTimer* _progress;   //进度条
    float _progressInterval;    //进度条更新次数
    int _sourceCount;   //资源总数
    
public:
    static Scene* createScene();
    virtual bool init();
    void load(float delta); //更新游戏进度，计算何时加载完成
    void menuCloseCallback(Ref* pSender);   //选择器回调函数
    CREATE_FUNC(Preload);
};

#endif /* PreloadScene_hpp */
