//
//  ContraWithOutBossLayer.cpp
//  RebornOfBall
//
//  Created by LiuGloria on 15/5/29.
//
//

#include "ContraWithOutBossLayer.h"
USING_NS_CC;


ContraWithOutBossLayer::ContraWithOutBossLayer(){
    //initialize the brick counter
    brick_count = 0;
    
    //contra_state = false;
}

ContraWithOutBossLayer::~ContraWithOutBossLayer(){
}

//bool ContraWithOutBossLayer::getContraState(){
//    return contra_state;
//}
//void ContraWithOutBossLayer::setContraState(bool state){
//    contra_state = state;
//}

//void ContraWithOutBossLayer::aFewSecondsSchedule(float dur){
//    //turn off the switch
//    setContraState(false);
//}


bool ContraWithOutBossLayer::init(){
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

void ContraWithOutBossLayer::initGame(){
    
    srand(time(NULL));
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    
    for (int i=0; i< COLLIDED_NUMBER; i++) {
        collidedBodies[i] = NULL;
    }
    
    for (int j = 0; j < CONTRA_BRICK_COUNT; j++) {
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
    
    listener->onTouchBegan = CC_CALLBACK_2(ContraWithOutBossLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ContraWithOutBossLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ContraWithOutBossLayer::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    
    //get the singleton
    st = ContraSingleton::GetInstance();
    
    initIndicator();

    
    scheduleUpdate();
    
}

void ContraWithOutBossLayer::initIndicator(){
    indicator = Sprite::create("boss_cry.png");
    indicator->setPosition(Vec2(visibleSize.width - 10 - indicator->getContentSize().width / 2,
                                visibleSize.height - 10 - indicator->getContentSize().height /2));
    addChild(indicator,-1);
    if (st->getContraState() == false) {
        indicator->setVisible(false);
    }
}

void ContraWithOutBossLayer::update(float dur){
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
    
    
    this->dealCollision(dur);
    
    for (int i = 0; i < CONTRA_BRICK_COUNT; i++) {
        bricks[i]->dealCollision(dur, monsterBody);
    }
    
    
    
    if (indicator) {
        if (st->getContraState() == false) {
            indicator->setVisible(false);
        }else if (st->getContraState() == true){
            indicator->setVisible(true);
        }
    }
    
    world->ClearForces();
    world->DrawDebugData();
}


bool ContraWithOutBossLayer::onTouchBegan( Touch* touch, Event *event )
{
    monsterBody->SetTransform(b2Vec2(572 / WORLDSCALE, 280 / WORLDSCALE), 0);
    initMonsterBody();
    
    auto location = touch->getLocation();
    
    //Vec2 relatedLocation = paddleNode->convertToNodeSpace(touch->getLocation());
    //if(paddle->getBoundingBox().containsPoint(relatedLocation))
    //{
    if(!m_bMouseDown){
        m_bMouseDown=true;
        m_vec2_1->updatefunc(location.x ,location.y);
        m_nAngle=0;
    }
    //}
    return true;
}


void ContraWithOutBossLayer::onTouchMoved( Touch* touch, Event *event)
{
    auto location = touch->getLocation();
    
    if(!m_bMouseDown){
        return;
    }
    
    m_vec2_2->updatefunc(location.x ,location.y);
    m_nAngle = Vector2::Angle(*m_vec2_1 , *m_vec2_2);//degree
    m_vec2_1->updatefunc(location.x ,location.y);
    
    
    float p_rotation = paddleNode->getRotation();
    
    if (st->getContraState() == true) {
        paddleNode->setRotation(p_rotation - m_nAngle);
    }else if(st->getContraState() == false){
        paddleNode->setRotation(p_rotation + m_nAngle);
    }
    
    
    Vec2 position = paddleNode->convertToWorldSpace(paddle->getPosition());
    b2Vec2 p_position = b2Vec2(position.x / WORLDSCALE, position.y / WORLDSCALE);
    float p_angle = -( CC_DEGREES_TO_RADIANS(paddleNode->getRotation()) );
    paddleBody->SetTransform(p_position,p_angle);
    
    m_nAngle = 0.0;
    
}


void ContraWithOutBossLayer::initBrickGroup()
{
    addSingleBrick(0, 8);
    addSingleBrick(1, 7);addSingleBrick(1, 9);
    addSingleBrick(2, 6);addSingleBrick(2, 8);addSingleBrick(2, 10);
    addSingleBrick(3, 5);addSingleBrick(3, 7);addSingleBrick(3, 9);addSingleBrick(3, 11);
    addSingleBrick(4, 4);addSingleBrick(4, 6);addSingleBrick(4, 8);addSingleBrick(4, 10);addSingleBrick(4, 12);
    addSingleBrick(5, 3);addSingleBrick(5, 5);addSingleBrick(5, 9);addSingleBrick(5, 11);addSingleBrick(5, 13);
    addSingleBrick(6, 2);addSingleBrick(6, 10);addSingleBrick(6, 12);addSingleBrick(6, 14);
    addSingleBrick(7, 3);addSingleBrick(7, 15);
    
    addSingleBrick(8, 1);addSingleBrick(8, 4);addSingleBrick(8, 16);
    
    addSingleBrick(9, 3);addSingleBrick(9, 15);
    addSingleBrick(10, 2);addSingleBrick(10, 10);addSingleBrick(10, 12);addSingleBrick(10, 14);
    addSingleBrick(11, 3);addSingleBrick(11, 5);addSingleBrick(11, 9);addSingleBrick(11, 11);addSingleBrick(11, 13);
    addSingleBrick(12, 4);addSingleBrick(12, 6);addSingleBrick(12, 8);addSingleBrick(12, 10);addSingleBrick(12, 12);
    addSingleBrick(13, 5);addSingleBrick(13, 7);addSingleBrick(13, 9);addSingleBrick(13, 11);
    addSingleBrick(14, 6);addSingleBrick(14, 8);addSingleBrick(14, 10);
    addSingleBrick(15, 7);addSingleBrick(15, 9);
    addSingleBrick(16, 8);
    
}

void ContraWithOutBossLayer::addSingleBrick(int i, int j){
    float x0 = visibleSize.width  /2 - SINGLE_BRICK_LENGTH * ROW_BRICK_NUMBERS / 2 + SINGLE_BRICK_LENGTH /2;
    float y0 = visibleSize.height /2 + SINGLE_BRICK_LENGTH * ROW_BRICK_NUMBERS / 2 - SINGLE_BRICK_LENGTH /2;
    
    
    if (bricks[brick_count] == NULL) {
        bricks[brick_count] = new BasicBrick(world,visibleSize,this);
        bricks[brick_count]->init(x0 + i * SINGLE_BRICK_LENGTH, y0 - j * SINGLE_BRICK_LENGTH, "brick_little_blue.png", "brick");
        bricks[brick_count]->initContraAttr();
        
        brick_count++;
    }
}
