//
//  GrowingBrickLayer.cpp
//  RebornOfBall
//
//  Created by LiuGloria on 15/5/24.
//
//

#include "GrowingBrickLayer.h"

USING_NS_CC;

GrowingBrickLayer::GrowingBrickLayer(){
    //calculate the number of bricks in the array
    growing_brick_count = 0;
}

GrowingBrickLayer::~GrowingBrickLayer(){
}

bool GrowingBrickLayer::init(){
    if(!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    // add the background
    auto sprite = Sprite::create("bg_growing.jpg");
    //sprite->setScale(0.5f);
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(sprite, -1);
    //sprite->setOpacity(127.5f);
    
    initGame();
    
    return true;
}

void GrowingBrickLayer::initGame(){
    
    srand(time(NULL));
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    
    for (int i=0; i< COLLIDED_NUMBER; i++) {
        collidedBodies[i] = NULL;
    }
    
    for (int j = 0; j < GROWING_BRICK_COUNT; j++) {
        bricks[j] = NULL;
    }
    
    
    float angle = MATH_DEG_TO_RAD(180);
    
    
    initPhysics();
    
    initDebugDraw();
    
    //draw the ray
    drawNode = DrawNode::create();
    addChild(drawNode);
    drawFriNode = DrawNode::create();
    addChild(drawFriNode);
    
    m_x1 = 0;
    m_y1 = 0;
    m_x2 = 0;
    m_y2 = 0;
    
    m_x = 0;
    m_y = 0;
    
    mReflectX = 0;
    mReflectY = 0;
    
    
    addPaddle();
    
    addMonsterBody();
    initMonsterBody();
    
    
    initBrickGroup();
    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(GrowingBrickLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GrowingBrickLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GrowingBrickLayer::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    scheduleUpdate();
    
}

void GrowingBrickLayer::update(float dur){
    int velIteration = 8;
    int postIteration = 3;
    world->Step(dur, velIteration, postIteration);
    
    //synchronizing its skin
    monster->setPosition(monsterBody->GetPosition().x * WORLDSCALE, monsterBody->GetPosition().y * WORLDSCALE);
    monster->setRotation(CC_RADIANS_TO_DEGREES(monsterBody->GetAngle()));
    
    
    //visualization
    calInterPoint();
    calReflectedRay();
    drawNode->clear();
    Vec2 dirVec = Vec2(monster->getPosition().x - paddleNode->getPosition().x, monster->getPosition().y - paddleNode->getPosition().y);
    float multiNum = dirVec.x * monsterBody->GetLinearVelocity().x + dirVec.y * monsterBody->GetLinearVelocity().y;
    if (/*multiNum > 0 &&*/ monster->getPosition().distance(paddleNode->getPosition()) > MONSTER_RADIUS
        && monster->getPosition().distance(paddleNode->getPosition()) < CIRCLE_RADIUS)
    {
        
        drawNode->drawSegment(Vec2(monster->getPosition().x, monster->getPosition().y), Vec2(m_x, m_y), 3.0f, Color4F(1, 1, 1, 1));
        drawNode->drawSegment(Vec2(m_x, m_y), Vec2(mReflectX, mReflectY), 3.0f, Color4F(1, 1, 1, 1));
    }
    
    /*String* monsterMass = String::createWithFormat("%f",monsterBody->GetMass());
     LabelTTF *label;
     label->setString(&monsterMass);
     label->setString(monsterBody->GetMass())
     LabelTTF *label = LabelTTF::labelWithString("hello world", CCSizeMake(100, 25), (CCTextAlignment)intAlign, strFontName, fontSize);
     label->setAnchorPoint(ccp(0, 0));
     label->setPosition = ccp(100,100);*/
    
    
    this->dealCollision(dur);
    
    for (int i = 0; i < GROWING_BRICK_COUNT; i++) {
        bricks[i]->dealCollision(dur, monsterBody);
    }
    
    
    world->ClearForces();
    world->DrawDebugData();
}



void GrowingBrickLayer::initBrickGroup(){
    for ( ; growing_brick_count < GROWING_BRICK_COUNT; growing_brick_count++) {
        if (bricks[growing_brick_count] == NULL) {
            bricks[growing_brick_count] = new BasicBrick(world,visibleSize,this);
            float posX,posY = 0.0f;
            switch (growing_brick_count) {
                case 0:{
                    posX = visibleSize.width/2;
                    posY = visibleSize.height/2 + GROWING_BRICK_GAP * 2;
                    break;}
                case 1:{
                    posX = visibleSize.width/2 - GROWING_BRICK_GAP;
                    posY = visibleSize.height/2 + GROWING_BRICK_GAP * 1;
                    break;}
                case 2:{
                    posX = visibleSize.width/2 + GROWING_BRICK_GAP;
                    posY = visibleSize.height/2 + GROWING_BRICK_GAP * 1;
                    break;}
                case 3:{
                    posX = visibleSize.width/2 - GROWING_BRICK_GAP * 2;
                    posY = visibleSize.height/2;
                    break;}
                case 4:{
                    posX = visibleSize.width/2 + GROWING_BRICK_GAP * 2;
                    posY = visibleSize.height/2;
                    break;}
                case 5:{
                    posX = visibleSize.width/2 - GROWING_BRICK_GAP * 1;
                    posY = visibleSize.height/2 - GROWING_BRICK_GAP * 1;
                    break;}
                case 6:{
                    posX = visibleSize.width/2 + GROWING_BRICK_GAP * 1;
                    posY = visibleSize.height/2 - GROWING_BRICK_GAP * 1;
                    break;}
                case 7:{
                    posX = visibleSize.width/2;
                    posY = visibleSize.height/2 - GROWING_BRICK_GAP * 2;
                    break;}
                default:{
                    posX = visibleSize.width/2;
                    posY = visibleSize.height/2;
                    break;}
            }
            bricks[growing_brick_count]->init(posX, posY,"growingBrick1.png", "brick", 1);
        }
    }
    

}
