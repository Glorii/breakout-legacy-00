//
//  PortalLayer.h
//  helloworld
//
//  Created by LiuGloria on 15/4/23.
//
//

#ifndef __helloworld__PortalLayer__
#define __helloworld__PortalLayer__

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

#define TOTAL_BRICK_COUNT 26
#define BRICK_COLUMN 3
#define BRICK_LENGTH 68
#define BRICK_GAP 2

//#define PADDLE_LENGTH 357
#define CIRCLE_RADIUS 241.5

#define MONSTER_SPEED 9


class PortalLayer:public BackgroundLayer{
public:
    PortalLayer();
    ~PortalLayer();
    virtual bool init();
    CREATE_FUNC(PortalLayer);
    
protected:
    Sprite* paddle2;//user data for circular paddle
    b2Body* paddleBody2;
    
    int brick_count;
    
    BasicBrick* bricks[TOTAL_BRICK_COUNT];
    
    float mReflectX_Portal;
    float mReflectY_Portal;
    
    float m_x_Portal;
    float m_y_Portal;
    
    virtual void initGame();
    
    virtual void addPaddle();
    
    virtual void initMonsterBody();
    
    virtual void update(float dur);
    virtual void dealCollision(float dur);
    
    virtual bool onTouchBegan(Touch* touch, Event *event);
    virtual void onTouchMoved(Touch* touch, Event *event);
    
    void calInterPoint_Portal();
    
	virtual void initBrickGroup();
	void addBrickLine(std::string filename,std::string name,int lineSequence,float marginScale,float num,float gapScale);
};


#endif /* defined(__helloworld__PortalLayer__) */
