//
//  NoisyLayer.h
//  RebornOfBall
//
//  Created by LiuGloria on 15/6/5.
//
//

#ifndef __RebornOfBall__NoisyLayer__
#define __RebornOfBall__NoisyLayer__

#include <stdio.h>

#include "BackgroundLayer.h"

USING_NS_CC;

#define NOISY_BRICK_COUNT 40
#define NOISY_BOSS_HEIGHT 90



class NoisyLayer:public BackgroundLayer{
public:
    NoisyLayer();
    ~NoisyLayer();
    virtual bool init();
    CREATE_FUNC(NoisyLayer);
    
protected:
    
    BasicBrick* bricks[NOISY_BRICK_COUNT];
    int brick_count;
    
    Sprite* bossFace;
    b2Body *bossBody;
    int boss_life_point;
    
    
    virtual void initGame();
    virtual void update(float dur);
    virtual void dealCollision(float dur);
    
    void dealBossDeletion(float dt);
    
    virtual void initBrickGroup();
    void generatingBricks(float dt);
    
    void addBoss();
    
    
    
};





#endif /* defined(__RebornOfBall__NoisyLayer__) */
