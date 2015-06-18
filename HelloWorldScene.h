#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "BackgroundLayer.h"
#include "BlacknWhiteLayer.h"
#include "PortalLayer.h"
#include "GravityLayer.h"
#include "ReverseLayer.h"
#include "GrowingBrickLayer.h"
#include "ContraLayer.h"
#include "ContraWithOutBossLayer.h"
#include "PiecesLayer.h"
#include "NoisyLayer.h"

class HelloWorld : public cocos2d::Scene
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    //static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
