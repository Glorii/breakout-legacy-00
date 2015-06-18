//
//  GravityLayer.h
//  RebornOfBall
//
//  Created by LiuGloria on 15/4/30.
//
//

#ifndef __RebornOfBall__GravityLayer__
#define __RebornOfBall__GravityLayer__

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
#define BRICK_LENGTH 68
#define BRICK_GAP 2

//#define  PADDLE_LENGTH 357
#define CIRCLE_RADIUS 330

#define PADDLE_THICKNESS 80

#define MONSTER_SPEED 10


#define FORCE_AREA 150.0f


class GravityLayer:public BackgroundLayer{
public:
    GravityLayer();
    ~GravityLayer();
    //virtual bool init();
    CREATE_FUNC(GravityLayer);
    
protected:
    Sprite* leftWing;
    b2Body* leftWingBody;
    Sprite* rightWing;
    b2Body* rightWingBody;
    
    //virtual void addPaddle();
    
    virtual void update(float dur);
    
    //virtual void onTouchMoved(Touch* touch, Event *event);

};







#endif /* defined(__RebornOfBall__GravityLayer__) */
