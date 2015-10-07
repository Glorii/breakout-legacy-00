//
//  DemoLayer.h
//  RebornOfBall
//
//  Created by LiuGloria on 15/7/25.
//
//

#ifndef __RebornOfBall__DemoLayer__
#define __RebornOfBall__DemoLayer__

#include <stdio.h>
#include "BlackholeLayer.h"

#define MONSTER_RADIUS 20/2.0f
#define BOARD_TRACE_RADUIS_MAX 225.0f
#define BOARD_TRACE_RADUIS_MIN 167.5f

#define ELASTIC_BOARD_COUNT 6

class DemoLayer : public BlackholeLayer
{
public:
    DemoLayer(void);
    ~DemoLayer(void);
    
    virtual bool init();
    CREATE_FUNC(DemoLayer);
    
protected:
    b2Body* elasticBoard[6];
    float boardDistance;
    float increment;
    virtual void onUpdate();
    
    
    virtual void initMonsterBody();
    virtual void initBrickGroup();
    
    void addBlackholes(float dt);
    
    void initBoardGroup(float dt);
    void deleteElasticBoards(float dt);
};


#endif /* defined(__RebornOfBall__DemoLayer__) */
