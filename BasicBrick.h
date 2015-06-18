//
//  BasicBrick.h
//  RebornOfBall
//
//  Created by LiuGloria on 15/5/13.
//
//

#ifndef __RebornOfBall__BasicBrick__
#define __RebornOfBall__BasicBrick__

#include <stdio.h>
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "cocos-ext.h"
#include "AppDelegate.h"
#include "GLES-Render.h"
#include "Vector2.h"
#include <math.h>
#include "ContraSingleton.h"

//#include "ContraWithOutBossLayer.h"

USING_NS_CC;


#define WORLDSCALE 30

#define COLLIDED_NUMBER 10

#define BRICK_COLUMN 3
#define BRICK_LENGTH 68
#define BRICK_GAP 2

#define GROWING_BRICK_LENGTH 28
#define GROWING_BRICK_HEIGHT 16
#define GROWING_BRICK_GAP 40

#define NOISY_BOSS_HEIGHT 90
#define NOISY_BRICK_COUNT 40

#define MONSTER_SPEED 10


class BasicBrick:public cocos2d::Node,b2ContactListener
{
public:
    Sprite *brick_cry;
    
    
    bool isSpiral;
    
    
    BasicBrick(b2World *world, Size visibleSize, Layer *m_layer);
    ~BasicBrick();
    virtual bool init(int i,int j);
    virtual bool init(float posx,float posy,std::string filename,std::string name);
    virtual bool init(float posx,float posy,std::string filename,std::string name,int lifePoint);
    
    virtual bool initWithCentralNode(int number,std::string filename,std::string name);
    virtual void initAsSpiral(float posx,float posy,std::string filename,std::string name);
    
    bool initACryingFace();
    bool initContraAttr();
    
    
    
    //CREATE_FUNC(BasicBrick);
    
    virtual void dealCollision(float dur,b2Body* m_monsterBody);
    
    std::string getAttr();
    
    void setLifePoint(int m_life);
    int getLifePoint();
    
    void spiralPosCallback(float dt);

    
protected:
    b2World *world;
    Size visibleSize;
    Layer *layer;
    
    Sprite *brick;
    b2Body *brickBody;
    b2Body *monsterBody;
    
    
    
    
    b2Vec2 oldVec;
    b2Vec2 brickForceVec;
    
    Node* centralNode;
    
    int lifePoint;
    
    std::string attr;

    
    
    float m_pos2_X;
    float m_pos2_Y;
    
    float m_pos3_X;
    float m_pos3_Y;
    
    int spiral_count;
    float spiral_degree;
    
    
    
    virtual void dealDeletion(float dt);
    virtual void dealRebirth_2(float dt);
    virtual void dealRebirth_3(float dt);
    
    void posChangingCallback(float dt);
    virtual void update(float dur);
    
    
};


#endif /* defined(__RebornOfBall__BasicBrick__) */
