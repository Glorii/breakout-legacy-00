//
//  ContraSingleton.h
//  RebornOfBall
//
//  Created by LiuGloria on 15/5/29.
//
//

#ifndef __RebornOfBall__ContraSingleton__
#define __RebornOfBall__ContraSingleton__

#include <stdio.h>

#include "cocos2d.h"
//#include "Box2D/Box2D.h"
#include "cocos-ext.h"





class ContraSingleton:public cocos2d::Node
{
public:
    static ContraSingleton* GetInstance();
    void setSchedule();
    void stopSchedule();
    
    bool getContraState();
    void setContraState(bool state);
private:
    ContraSingleton() {}
    static ContraSingleton *singleton;
    

    
    void scheduleCallback(float dur);
    
    bool contra_state = false;
};

#endif /* defined(__RebornOfBall__ContraSingleton__) */
