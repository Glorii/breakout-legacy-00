//
//  ContraLayer.h
//  RebornOfBall
//
//  Created by LiuGloria on 15/5/25.
//
//

#ifndef __RebornOfBall__ContraLayer__
#define __RebornOfBall__ContraLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "cocos-ext.h"
#include "AppDelegate.h"
#include "GLES-Render.h"
#include "Vector2.h"
#include <math.h>
#include "BackgroundLayer.h"

USING_NS_CC;

#define CONTRA_BRICK_COUNT 55
#define SINGLE_BRICK_LENGTH 32
#define ROW_BRICK_NUMBERS 17




class ContraLayer:public BackgroundLayer{
public:
    ContraLayer();
    ~ContraLayer();
    virtual bool init();
    CREATE_FUNC(ContraLayer);
protected:
    
    BasicBrick* bricks[CONTRA_BRICK_COUNT];
    int brick_count;
    
    Sprite *boss_smile;
    Sprite *boss_cry;
    
    bool boss_cry_state;
    
    b2Body *bossBody;
    
    int boss_life_point;
    
    
    
    virtual void initGame();
    
    virtual void update(float dur);
    virtual void dealCollision(float dur);
    
    void dealBossDeletion(float dt);
    void faceChanging(float dt);
    
    
    virtual bool onTouchBegan(Touch* touch, Event *event);
    virtual void onTouchMoved(Touch* touch, Event *event);
    
    virtual void initBrickGroup();
    
    void addBoss();
    
    void addSingleBrick(int i, int j);
    
    
};

#endif /* defined(__RebornOfBall__ContraLayer__) */
