//
//  ContraWithOutBossLayer.h
//  RebornOfBall
//
//  Created by LiuGloria on 15/5/29.
//
//

#ifndef __RebornOfBall__ContraWithOutBossLayer__
#define __RebornOfBall__ContraWithOutBossLayer__

#include <stdio.h>

#include "BackgroundLayer.h"
//#include "ContraSingleton.h"

USING_NS_CC;

#define CONTRA_BRICK_COUNT 55
#define SINGLE_BRICK_LENGTH 32
#define ROW_BRICK_NUMBERS 17



class ContraWithOutBossLayer : public BackgroundLayer{
public:
    ContraWithOutBossLayer();
    ~ContraWithOutBossLayer();
    virtual bool init();
    CREATE_FUNC(ContraWithOutBossLayer);
    
    //static bool getContraState();
    //static void setContraState(bool state);
    
    //static void aFewSecondsSchedule(float dur);
protected:
    
    BasicBrick* bricks[CONTRA_BRICK_COUNT];
    int brick_count;
    
    Sprite* indicator;
    
    ContraSingleton* st;
    
    //static bool contra_state;
    
    
    
    
    virtual void initGame();
    
    virtual void update(float dur);
    
    
    
    
    virtual bool onTouchBegan(Touch* touch, Event *event);
    virtual void onTouchMoved(Touch* touch, Event *event);
    
    virtual void initBrickGroup();
    
    void initIndicator();
    
    void addSingleBrick(int i, int j);
    
    
};


#endif /* defined(__RebornOfBall__ContraWithOutBossLayer__) */
