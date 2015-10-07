//
//  DemoLayer.cpp
//  RebornOfBall
//
//  Created by LiuGloria on 15/7/25.
//
//

#include "DemoLayer.h"


DemoLayer::DemoLayer(void)
{
}


DemoLayer::~DemoLayer(void)
{
}

bool DemoLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // add the background
    auto sprite = Sprite::create("bg_blackhole.jpg");
    //sprite->setOpacity();
    // position the sprite on the center of the screen
    sprite->setPosition(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    initGame();
    
    return true;
}

void DemoLayer::initMonsterBody(){
    if (monster)
    {
        removeChild(monster);
    }
    monster  = NULL;
    
    //monster = rand()%2 == 0 ? Sprite::create("whiteBall.png") : Sprite::create("blackBall.png");
    monster = Sprite::create("ball_demo2.png");
    Size monsterSize = monster->getContentSize();
    
    monster->setPosition(monsterBody->GetPosition().x * WORLDSCALE, monsterBody->GetPosition().y * WORLDSCALE);
    addChild(monster);
    monsterBody->SetUserData(monster);
    
    
    float angle;
    switch (rand()%5)
    {
        case 0:angle = CC_DEGREES_TO_RADIANS(90 - rand()%30);break;
        case 1:
        case 2:angle = CC_DEGREES_TO_RADIANS(135 - rand()%45);break;
        default: angle = CC_DEGREES_TO_RADIANS(180 - rand()%45);break;
    }
    monsterBody->SetLinearVelocity(b2Vec2(10 * cos(angle),-10 * sin(angle)));
}

void DemoLayer::initBrickGroup()
{
    for (int i=0; i<ELASTIC_BOARD_COUNT; i++) {
        elasticBoard[i] = nullptr;
    }
    for (int j=0; j<4; j++) {
        blackHoleEntry[j] = nullptr;
        blackHoleExit[j] = nullptr;
    }
    
    //add the elastic boards
    this->scheduleOnce(schedule_selector(DemoLayer::initBoardGroup),20.0f);
    //delete elastic boards
    this->scheduleOnce(schedule_selector(DemoLayer::deleteElasticBoards),30.0f);
    //add blackholes
    this->scheduleOnce(schedule_selector(DemoLayer::addBlackholes),35.0f);
    

    
    
    //brick
    addCoupleTriangularBrick(visibleSize.width/2,visibleSize.height/2 + 3.0f * TRIANGULAR_BRICK_SIDE_LENGTH * cos(CC_DEGREES_TO_RADIANS(30))  ,0);
    
    addBrickLine(0,0.5,2,1,0);
    addBrickLine(1, 0,3,1,0);
    
    //addBrickLine(3,0.5,1,1,180);
    addBrickLineOnAngle(2,0,0,1,0);
    addCoupleTriangularBrick(visibleSize.width/2,visibleSize.height/2 + 3.0f * TRIANGULAR_BRICK_SIDE_LENGTH * cos(CC_DEGREES_TO_RADIANS(30))  ,180);
    ///////////////boss
    bossEyeWhite = Sprite::create("bossEyeWhite.png");
    bossEyeWhite->setPosition(visibleSize.width/2,visibleSize.height/2);
    bossEyeWhite->setScale(0.4);
    bossEyeWhite->setTag(3);
    bossEyeWhite->setName("boss");
    addChild(bossEyeWhite);
    
    bossEyeBlack = Sprite::create("bossEyeBlack.png");
    bossEyeBlack->setPosition(visibleSize.width/2,visibleSize.height/2);
    bossEyeBlack->setScale(0.4);
    this->addChild(bossEyeBlack);
    
    b2BodyDef bossBodyDef;
    bossBodyDef.type = b2_staticBody;
    
    bossBodyDef.position = b2Vec2( bossEyeWhite->getPosition().x / WORLDSCALE, bossEyeWhite->getPosition().y / WORLDSCALE);
    bossBody = world->CreateBody(&bossBodyDef);
    bossBody->SetUserData(bossEyeWhite);
    
    b2CircleShape bossShape;
    bossShape.m_radius = 123*0.4/2/WORLDSCALE;
    
    b2FixtureDef bossFixDef;
    bossFixDef.shape = &bossShape;
    bossFixDef.density = 1.0f;
    bossFixDef.friction = 0.3f;
    bossFixDef.restitution = 0.2f;
    
    bossBody->CreateFixture(&bossFixDef);
}

//add the blackholes
void DemoLayer::addBlackholes(float dt){
    ableToAttackBoss =true;
    
    for (int i = 0;i < 4;i ++)
    {
        blackHoleEntry[i] = Sprite::create(BLACKHOLE_ENTRY_FILENAME);
        blackHoleEntry[i]->setPosition(visibleSize.width/2 + CIRCLE_RADIUS/2,visibleSize.height/2);
        blackHoleEntry[i]->setScale(0.3);
        blackHoleEntry[i]->setTag(1);
        addChild(blackHoleEntry[i]);
        
        if (i == 1 || i ==3)
        {
            blackHoleEntry[i]->setTag(0);
            blackHoleEntry[i]->setVisible(false);
        }
        
    }
    
    nextDegree = 0;
    awakedBlackHole = 5;

}

void DemoLayer::initBoardGroup(float dt)
{
    boardDistance = BOARD_TRACE_RADUIS_MIN;
    for (int i = 0;i < ELASTIC_BOARD_COUNT;i++)
    {
        auto board = Sprite::create("board.png");
        board->setScaleX(0.75);
        Size boardSize = board->getContentSize();
        board->setName("wall");
        board->setRotation(330 - 60 * i);
        
        float posX = visibleSize.width/2 + BOARD_TRACE_RADUIS_MAX *sin(CC_DEGREES_TO_RADIANS(board->getRotation()));
        float posY = visibleSize.height/2 +BOARD_TRACE_RADUIS_MAX *cos(CC_DEGREES_TO_RADIANS(board->getRotation()));
        board->setPosition(posX,posY);
        addChild(board);
        
        b2BodyDef boardBodyDef;
        boardBodyDef.type = b2_staticBody;
        boardBodyDef.position = b2Vec2(posX/WORLDSCALE,posY/WORLDSCALE);
        boardBodyDef.angle =  CC_DEGREES_TO_RADIANS(- 330 + 60 * i);
        
        b2PolygonShape boardShape;
        boardShape.SetAsBox( boardSize.width*board->getScaleX() / 2 / WORLDSCALE,boardSize.height/ 2 / WORLDSCALE);
        
        b2FixtureDef boardFixDef;
        boardFixDef.shape = &boardShape;
        boardFixDef.density = 10.0f;
        boardFixDef.friction = 0.3;
        boardFixDef.restitution = 0.2f;
        
        b2Body *boardBody = world->CreateBody(&boardBodyDef);
        boardBody->CreateFixture(&boardFixDef);
        
        boardBody->SetUserData(board);
        
        //boardBody->SetTransform,CC_DEGREES_TO_RADIANS(board->getRotation() ) );
        
        elasticBoard[i] = boardBody;
    }
}

void DemoLayer::deleteElasticBoards(float dt){
    b2Body *bodyToDelete;
    Sprite* sp;
    
    for (int i=0; i<ELASTIC_BOARD_COUNT; i++) {
        if (elasticBoard[i]) {
            bodyToDelete = elasticBoard[i];
            if (bodyToDelete->GetUserData()) {
                sp = (Sprite*)bodyToDelete->GetUserData();
                
                world->DestroyBody(bodyToDelete);
                removeChild(sp);
                sp = NULL;
            }
        }
        
    }
    for (int i=0; i<ELASTIC_BOARD_COUNT; i++) {
        if (elasticBoard[i]) {
            elasticBoard[i] = NULL;
        }
        
    }

    
    
}

void DemoLayer::onUpdate()
{
    
    
    
    //elastic boards
    if (boardDistance <= BOARD_TRACE_RADUIS_MIN)
    {
        increment = 1;
    }
    if (boardDistance >= BOARD_TRACE_RADUIS_MAX)
    {
        increment = -1;
    }
    boardDistance += increment;
    
    for (int i = 0;i < ELASTIC_BOARD_COUNT;i++)
    {
        if (elasticBoard[i]) {
            if (elasticBoard[i]->GetUserData()) {
                Sprite* board = (Sprite*)elasticBoard[i]->GetUserData();
                float angle = CC_DEGREES_TO_RADIANS(board->getRotation());
                float posX = visibleSize.width/2 + boardDistance * sin(angle);
                float posY = visibleSize.height/2 + boardDistance * cos(angle);
                
                elasticBoard[i]->SetTransform(b2Vec2(posX/WORLDSCALE,posY/WORLDSCALE),elasticBoard[i]->GetAngle() );
                
                board->setPosition(posX,posY);
            }
            
        }
        
    }
    
    
    
    if (bossEyeBlack)
    {
        if (bossEyeWhite->getTag())
        {
            Vec2 dir = monster->getPosition() - Vec2(visibleSize.width/2,visibleSize.height/2);
            dir.normalize();
            bossEyeBlack->setPosition(dir * BOSS_EYE_RADIUS +  Vec2(visibleSize.width/2,visibleSize.height/2));
        }else
        {
            removeChild(bossEyeBlack);
            bossEyeBlack = NULL;
            bossEyeWhite = NULL;
            bossBody  = NULL;
        }
    }
    
    
    //float blackHoleRadius = CIRCLE_RADIUS / 3;
    for (int i = 0;i < 4;i++)
    {
        Sprite* m_blackhole = blackHoleEntry[i];
        if (m_blackhole != NULL)
        {
            if (nextDegree>360)
            {
                nextDegree = nextDegree - 360;
            }
            nextDegree += BLACKHOLE_ROTATE_VELCOCITY;
            float posx = cos(CC_DEGREES_TO_RADIANS(nextDegree + i*90))*BLACKHOLE_TRACK_RADIUS + visibleSize.width/2;
            float posy = sin(CC_DEGREES_TO_RADIANS(nextDegree + i*90))*BLACKHOLE_TRACK_RADIUS + visibleSize.height/2;
            m_blackhole->setPosition(posx,posy);
        }
    }
    
    for (int i = 0;i<4;i++)
    {
        blackHoleExit[i] = NULL;
        
        Sprite* m_blackhole = blackHoleEntry[i];
        if (m_blackhole) {
            if (m_blackhole->getTag() == 1 && i != awakedBlackHole)
            {
                if (monster->getPosition().distance(m_blackhole->getPosition()) <= BLACKHOLE_RADIUS && awakedBlackHole == 5)
                {
                    monsterBody->SetAwake(false);
                    monster->setVisible(false);
                    //m_blackhole->setVisible(false);
                    //this->seq
                    awakedBlackHole = i;
                    /*DelayTime * delayAction = DelayTime::create(0.5f);
                     CallFunc * callFunc = CallFunc::create(randomAppear);
                     this->runAction(Sequence::createWithTwoActions(delayAction, callFunc));
                     this->runAction(Sequence::createWithTwoActions(delayAction,  CallFunc::create(PopBall)));*/
                    //this->runAction(Sequence(randomAppear,PopBall))
                    this->scheduleOnce(schedule_selector(DemoLayer::randomAppear),0.2f);
                    this->scheduleOnce(schedule_selector(DemoLayer::PopBall),0.5f);
                    
                    return;
                }
            }
            
        }
        
        
    }
}

