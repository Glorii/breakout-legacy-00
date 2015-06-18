//
//  GrowingBrickLayer.h
//  RebornOfBall
//
//  Created by LiuGloria on 15/5/24.
//
//

#ifndef __RebornOfBall__GrowingBrickLayer__
#define __RebornOfBall__GrowingBrickLayer__

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


#define WORLDSCALE 30
//#define SEMI_SIRCLE_RADIUS 255
#define MONSTER_RADIUS 30/2.0f
#define MONSTER_FRICTION 0.8

#define COLLIDED_NUMBER 10

#define BRICK_COUNT 26
#define BRICK_COLUMN 3
#define BRICK_LENGTH 68
#define BRICK_GAP 2

#define GROWING_BRICK_LENGTH 28
#define GROWING_BRICK_HEIGHT 16
#define GROWING_BRICK_GAP 64


#define GROWING_BRICK_COUNT 8


//#define  PADDLE_LENGTH 357
#define CIRCLE_RADIUS 241.5

#define MONSTER_SPEED 9

class GrowingBrickLayer:public BackgroundLayer{
public:
    GrowingBrickLayer();
    ~GrowingBrickLayer();
    virtual bool init();
    CREATE_FUNC(GrowingBrickLayer);
    
protected:
    BasicBrick* bricks[GROWING_BRICK_COUNT];
    
    int growing_brick_count;
    
    virtual void update(float dur);
    
    virtual void initGame();
    
    virtual void initBrickGroup();

    
};


#endif /* defined(__RebornOfBall__GrowingBrickLayer__) */
