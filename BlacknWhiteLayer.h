//
//  BlacknWhiteLayer.h
//  helloworld
//
//  Created by LiuGloria on 15/4/17.
//
//

#ifndef __helloworld__BlacknWhiteLayer__
#define __helloworld__BlacknWhiteLayer__

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
#define SEMI_SIRCLE_RADIUS 255
#define MONSTER_RADIUS 35/2.0f
#define MONSTER_FRICTION 0.8

#define COLLIDED_NUMBER 10

#define BRICK_COLUMN 3
#define BRICK_COUNT 9
#define BRICK_LENGTH 68
#define BRICK_GAP 2

#define  PADDLE_LENGTH 357
#define CIRCLE_RADIUS 357

#define MONSTER_SPEED 10



class BlacknWhiteLayer:public BackgroundLayer{
    
public:
    BlacknWhiteLayer();
    ~BlacknWhiteLayer();
    //virtual bool init();
    CREATE_FUNC(BlacknWhiteLayer);
    
protected:
    Sprite* paddle2;//user data for circular paddle
    b2Body* paddleBody2;
    
    float mReflectX_Black;
    float mReflectY_Black;
    
    float m_x_Black;
    float m_y_Black;

    
    virtual void addPaddle();
    
    virtual void addMonsterBody();
    virtual void initMonsterBody();
    
    
    virtual void update(float dur);
    virtual void dealCollision(float dur);
    
    void calInterPoint_Black();
    void calReflectedRay_Black();
    
    
    virtual bool onTouchBegan(Touch* touch, Event *event);
    virtual void onTouchMoved(Touch* touch, Event *event);

};

#endif /* defined(__helloworld__BlacknWhiteLayer__) */
