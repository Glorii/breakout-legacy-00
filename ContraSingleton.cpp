//
//  ContraSingleton.cpp
//  RebornOfBall
//
//  Created by LiuGloria on 15/5/29.
//
//

#include "ContraSingleton.h"


ContraSingleton* ContraSingleton::singleton = NULL;
ContraSingleton* ContraSingleton::GetInstance()
{
    if(singleton == NULL)
        singleton = new ContraSingleton();
    return singleton;
}

bool ContraSingleton::getContraState(){
    return contra_state;
}
void ContraSingleton::setContraState(bool state){
    cocos2d::CCNode::onEnter();
    
    contra_state = state;
}


void ContraSingleton::setSchedule(){
    this->scheduleOnce(schedule_selector(ContraSingleton::scheduleCallback), 5.0f);
}

void ContraSingleton::stopSchedule(){
    this->unschedule(schedule_selector(ContraSingleton::scheduleCallback));
}

void ContraSingleton::scheduleCallback(float dur){
    this->setContraState(false);
}





