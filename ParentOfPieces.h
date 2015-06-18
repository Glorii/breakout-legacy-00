//
//  ParentOfPieces.h
//  RebornOfBall
//
//  Created by LiuGloria on 15/5/30.
//
//

#ifndef __RebornOfBall__ParentOfPieces__
#define __RebornOfBall__ParentOfPieces__

#include <stdio.h>
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "cocos-ext.h"
#include "AppDelegate.h"
#include "GLES-Render.h"
#include "Vector2.h"
#include <math.h>

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
#define CIRCLE_RADIUS 357

#define PADDLE_THICKNESS 80

#define MONSTER_SPEED 10


class ParentOfPieces:public cocos2d::Layer,b2ContactListener
{
public:
    ParentOfPieces();
    ~ParentOfPieces();
    virtual bool init();
    CREATE_FUNC(ParentOfPieces);
    
protected:
    b2World *world;
    Sprite *monster;
    b2Body *monsterBody;
    Size visibleSize;
    
    //collison
    Vec2 dirVec;
    b2Vec2 forceVec;
    b2Vec2 frictionVec;
    b2Vec2 tanVec;
    b2Vec2 oldVec;
    b2Vec2 brickForceVec;
    
    
    b2Body* collidedBodies[COLLIDED_NUMBER];
    
    
    
    //paddle
    Vector2 *m_vec2_1;
    Vector2 *m_vec2_2;
    bool m_bMouseDown;
    float m_nAngle;
    bool t_bInit;
    Sprite* paddle;//user data for circular paddle
    //  Sprite* clip;
    //new flat paddle
    Node* paddleNode;
    b2Body* paddleBody;
    float rotation;
    
    
    DrawNode* drawNode;
    DrawNode* drawFriNode;
    float m_x1;
    float m_y1;
    float m_x2;
    float m_y2;
    
    float m_x;
    float m_y;
    
    float mReflectX;
    float mReflectY;
    
    
    
    GLESDebugDraw *debugDraw;
    
    void initGame();
    void initPhysics();
    void initDebugDraw();
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    
    virtual void update(float dur);
    
    //touch event handler
    bool onTouchBegan(Touch* touch, Event *event);
    virtual void onTouchMoved(Touch* touch, Event *event);
    void onTouchEnded(Touch* touch, Event *event);
    
    
    //collision
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    
    virtual void addMonsterBody();
    virtual void initMonsterBody();
    virtual void dealCollision(float dur);
    
    
    //draw the line
    virtual void calInterPoint();
    virtual void calReflectedRay();
    
    //////////new
    virtual void onUpdate();
    virtual void onDealCollision(b2Body* collidedBody);
    
    
    void dealDeletion(float dt);
    
    //to be rewritten
    virtual void addPaddle();
    void addBrick(int i, int j);
    void addBrick(float posx,float posy,std::string filename,std::string name);
    
    virtual void initBrickGroup();
    void removeJudgment(b2Body* collidedBody);
};




#endif /* defined(__RebornOfBall__ParentOfPieces__) */
