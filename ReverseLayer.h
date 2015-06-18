//
//  ReverseLayer.h
//  RebornOfBall
//
//  Created by LiuGloria on 15/5/8.
//
//

#ifndef __RebornOfBall__ReverseLayer__
#define __RebornOfBall__ReverseLayer__

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
#define MONSTER_RADIUS 30/2.0f
#define MONSTER_FRICTION 0.8

#define COLLIDED_NUMBER 10


#define BRICK_COLUMN 3
#define BRICK_LENGTH 40
#define BRICK_GAP 0
#define BAFFLE_WIDTH 80
#define BAFFLE_HEIGHT 30

//#define CIRCLE_RADIUS 330
#define CIRCLE_RADIUS 237

#define PADDLE_THICKNESS 40

#define MONSTER_SPEED 10


class ReverseLayer:public BackgroundLayer{
public:
    ReverseLayer();
    ~ReverseLayer();
    virtual bool init();
    CREATE_FUNC(ReverseLayer);
    
protected:
    Sprite* leftWing;
    b2Body* leftWingBody;
    Sprite* rightWing;
    b2Body* rightWingBody;
    
    Sprite* disc;
    
    Node* brickGroupNode;
    
    int rotate_count;
    bool rotate_switch;
    
//    Vec2 brickGroupNodePos;
//    b2Vec2 b_group_pos;
    Sprite* m_sprite;
    float b_group_angle;
    
    
    virtual void initGame();
    
    virtual void initMonsterBody();
    virtual void addPaddle();
    virtual void addBrick(int i, int j);
    void addHorizontalBaffles(float pos_x, float pos_y);
    void addVerticalBaffles(float pos_x, float pos_y);
    
    virtual void update(float dur);
    
    
    virtual void dealCollision(float dur);
    virtual void dealDeletion(float dt);
    
    virtual void onTouchMoved(Touch* touch, Event *event);
    
    virtual void initBrickGroup();
    
};








#endif /* defined(__RebornOfBall__ReverseLayer__) */
