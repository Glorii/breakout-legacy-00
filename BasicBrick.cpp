//
//  BasicBrick.cpp
//  RebornOfBall
//
//  Created by LiuGloria on 15/5/13.
//
//

#include "BasicBrick.h"


USING_NS_CC;


BasicBrick::BasicBrick(b2World *world, Size visibleSize, Layer *m_layer){
    this->world = world;
    this->visibleSize = visibleSize;
    
    this->layer = m_layer;
    
    //set the lifepoint to the default value 0
    this->lifePoint = 0;
    
    this->centralNode = NULL;
    
    m_pos2_X = 0;
    m_pos2_Y = 0;
    m_pos3_X = 0;
    m_pos3_Y = 0;
    
    spiral_count = 0;
    spiral_degree = 11.0F / 12;
    isSpiral = false;
    
    CCNode::onEnter();
}

BasicBrick::~BasicBrick(){
}

void BasicBrick::setLifePoint(int m_life){
    this->lifePoint = m_life;
}

int BasicBrick::getLifePoint(){
    return this->lifePoint;
}


bool BasicBrick::init(int i,int j){
    this->brick = Sprite::create("brick.png");
    Size brickSize = this->brick->getContentSize();
    
    b2BodyDef brickBodyDef;
    brickBodyDef.type = b2_staticBody;
    
    float disX = visibleSize.width /2 - (BRICK_COLUMN * BRICK_LENGTH + (BRICK_COLUMN - 1) * BRICK_GAP) /2 ;
    float disY = visibleSize.height /2 - (BRICK_COLUMN * BRICK_LENGTH + (BRICK_COLUMN - 1) * BRICK_GAP) /2 ;
    
    brickBodyDef.position = b2Vec2( (disX + i * (BRICK_LENGTH + BRICK_GAP)) / WORLDSCALE, (disY + j * (BRICK_LENGTH + BRICK_GAP)) / WORLDSCALE);
    this->brickBody = world->CreateBody(&brickBodyDef);
    
    this->brick->setPosition(brickBodyDef.position.x * WORLDSCALE, brickBodyDef.position.y * WORLDSCALE);
    layer->addChild(this->brick,-1);
    
    this->brickBody->SetUserData(this->brick);
    
    this->brick->setName("brick");
    
    b2PolygonShape brickShape;
    brickShape.SetAsBox(brickSize.width / 2 / WORLDSCALE, brickSize.height / 2 / WORLDSCALE);
    b2FixtureDef brickFixDef;
    brickFixDef.shape = &brickShape;
    brickFixDef.density = 1.0f;
    brickFixDef.friction = 0.3f;
    brickFixDef.restitution = 0.2f;
    this->brickBody->CreateFixture(&brickFixDef);

    
    this->scheduleUpdate();
    return true;
}

bool BasicBrick::init(float posx,float posy,std::string filename,std::string name){
    this->brick = Sprite::create(filename);
    Size brickSize = this->brick->getContentSize();
    
    b2BodyDef brickBodyDef;
    brickBodyDef.type = b2_staticBody;
    
    brickBodyDef.position = b2Vec2(posx / WORLDSCALE,posy / WORLDSCALE);
    this->brickBody = world->CreateBody(&brickBodyDef);
    
    this->brick->setPosition(brickBodyDef.position.x * WORLDSCALE, brickBodyDef.position.y * WORLDSCALE);
    if (isSpiral) {
        this->brick->setVisible(false);
    }
    layer->addChild(this->brick,-1);
    
    
    this->brickBody->SetUserData(this->brick);
    
    this->brick->setName(name);
    
    b2PolygonShape brickShape;
    brickShape.SetAsBox(brickSize.width / 2 / WORLDSCALE, brickSize.height / 2 / WORLDSCALE);
    b2FixtureDef brickFixDef;
    brickFixDef.shape = &brickShape;
    brickFixDef.density = 1.0f;
    brickFixDef.friction = 0.3f;
    brickFixDef.restitution = 0.2f;
    this->brickBody->CreateFixture(&brickFixDef);

    this->scheduleUpdate();
    return true;
}

void BasicBrick::initAsSpiral(float posx,float posy,std::string filename,std::string name){
    isSpiral = true;
    init(posx, posy, filename, name);
    //rotate and move outer
    this->schedule(schedule_selector(BasicBrick::spiralPosCallback), 0.016F, CC_REPEAT_FOREVER, 0.0F);
    
}


bool BasicBrick::init(float posx,float posy,std::string filename,std::string name, int lifePoint){
    this->lifePoint = lifePoint;
    
    this->brick = Sprite::create(filename);
    Size brickSize = this->brick->getContentSize();
    
    b2BodyDef brickBodyDef;
    brickBodyDef.type = b2_staticBody;
    
    brickBodyDef.position = b2Vec2(posx / WORLDSCALE,posy / WORLDSCALE);
    this->brickBody = world->CreateBody(&brickBodyDef);
    
    this->brick->setPosition(brickBodyDef.position.x * WORLDSCALE, brickBodyDef.position.y * WORLDSCALE);
    layer->addChild(this->brick,-1);
    
    this->brickBody->SetUserData(this->brick);
    
    this->brick->setName(name);
    
    b2PolygonShape brickShape;
    brickShape.SetAsBox(brickSize.width / 2 / WORLDSCALE, brickSize.height / 2 / WORLDSCALE);
    b2FixtureDef brickFixDef;
    brickFixDef.shape = &brickShape;
    brickFixDef.density = 1.0f;
    brickFixDef.friction = 0.3f;
    brickFixDef.restitution = 0.2f;
    this->brickBody->CreateFixture(&brickFixDef);
    
    this->scheduleUpdate();
    return true;
}

bool BasicBrick::initACryingFace(){
    this->brick_cry = Sprite::create("brick_little_red.png");
    this->brick_cry->setPosition(this->brick->getPosition());
    layer->addChild(this->brick_cry,-1);
    this->brick_cry->setVisible(false);
    
    return true;
}

//init a brick with 5-second-contra-control attribute
bool BasicBrick::initContraAttr(){
    int number = rand() % 7;
    if (number <= 1) {
        this->attr = std::string("contra");
        
        this->brick_cry = Sprite::create("brick_little_red.png");
        this->brick_cry->setPosition(this->brick->getPosition());
        layer->addChild(this->brick_cry,-1);
    }
    return true;
}

std::string BasicBrick::getAttr(){
    return this->attr;
}


bool BasicBrick::initWithCentralNode(int number, std::string filename, std::string name){
    centralNode = Node::create();
    centralNode->setPosition(Vec2(visibleSize.width /2 , visibleSize.height /2));
    
    this->brick = Sprite::create(filename);
    Size brickSize = this->brick->getContentSize();
    centralNode->addChild(this->brick);
    this->brick->setPosition(0, NOISY_BOSS_HEIGHT / 2 + 24 + brickSize.height / 2);
    layer->addChild(centralNode);
    
    b2BodyDef brickBodyDef;
    brickBodyDef.type = b2_staticBody;
    
    Vec2 position = centralNode->convertToWorldSpace(this->brick->getPosition());
    
    brickBodyDef.position = b2Vec2(position.x / WORLDSCALE,position.y / WORLDSCALE);
    this->brickBody = world->CreateBody(&brickBodyDef);
    
    //this->brick->setPosition(brickBodyDef.position.x * WORLDSCALE, brickBodyDef.position.y * WORLDSCALE);
    
    
    this->brickBody->SetUserData(this->brick);
    
    this->brick->setName(name);
    
    b2PolygonShape brickShape;
    brickShape.SetAsBox(brickSize.width / 2 / WORLDSCALE, brickSize.height / 2 / WORLDSCALE);
    b2FixtureDef brickFixDef;
    brickFixDef.shape = &brickShape;
    brickFixDef.density = 1.0f;
    brickFixDef.friction = 0.3f;
    brickFixDef.restitution = 0.2f;
    this->brickBody->CreateFixture(&brickFixDef);
    
    this->scheduleUpdate();
    //rotate and move outer
    this->schedule(schedule_selector(BasicBrick::posChangingCallback), 1.F, CC_REPEAT_FOREVER, 0.0F);
    
    
    CCLOG("== noisy brick generated!==");
    return true;
}

void BasicBrick::spiralPosCallback(float dt){
    this->brick->setVisible(true);
    
    float initX = visibleSize.width/2;
    float initY = visibleSize.height/2 /*+ NOISY_BOSS_HEIGHT /2 + 24*/;
    
    float r = (45/* - spiral_degree * 8*/) * (1 + spiral_degree);
    float x = r * cosf(CC_DEGREES_TO_RADIANS(spiral_degree * 360) );
    float y = r * sinf(CC_DEGREES_TO_RADIANS(spiral_degree * 360) );
    
    float oldX = this->brick->getPosition().x;
    float oldY = this->brick->getPosition().y;
    
    float k = (y - oldY) / (x - oldX);
    
    
    float newX = oldX + 10 * cosf(atanf(k));
    float newY = oldY + 10 * sinf(atanf(k));
    
    
    
    float oldRotation = this->brick->getRotation();
    
    this->brick->setPosition(initX + x, initY + y);
    this->brick->setRotation(oldRotation );
    
    
    //spiral_degree += 1.0F / 12 * (1 - 1 / NOISY_BRICK_COUNT * spiral_count) * (1 - 1 / NOISY_BRICK_COUNT * spiral_count);
    
    //spiral_degree += 1.0F / 12 * (1-1/40 * spiral_count) * (1-1/40 * spiral_count);
    spiral_degree += 1.0F / (600 + spiral_count/20);
    spiral_count += 1;
}

void BasicBrick::posChangingCallback(float dt){
    CCLOG("==rotate and move!==");
    
    float oldRotation = this->centralNode->getRotation();
    this->centralNode->setRotation(oldRotation + 30.0F);
    
   
    float oldPosX = this->brick->getPosition().x;
    float oldPosY = this->brick->getPosition().y;
    this->brick->setPosition(Vec2(oldPosX /*- 1.2 * spiral_count*/, oldPosY + this->brick->getContentSize().height / 8));
    
    spiral_count += 1;
}

void BasicBrick::update(float dur){
    if (centralNode) {
        
        Vec2 position = centralNode->convertToWorldSpace(this->brick->getPosition());
        b2Vec2 p_position = b2Vec2(position.x / WORLDSCALE, position.y / WORLDSCALE);
        float p_angle = -( CC_DEGREES_TO_RADIANS(centralNode->getRotation()) );
        this->brickBody->SetTransform(p_position,p_angle);
    }
    
    if (isSpiral) {
        if (this->brick) {
            b2Vec2 p_position = b2Vec2(this->brick->getPosition().x / WORLDSCALE, this->brick->getPosition().y / WORLDSCALE);
            float p_angle = -( CC_DEGREES_TO_RADIANS(this->brick->getRotation()) );
            this->brickBody->SetTransform(p_position,p_angle);
        }
        
        
    }
    

}


void BasicBrick::dealCollision(float dur,b2Body* m_monsterBody){
    monsterBody = m_monsterBody;
    
    if (this->brickBody != NULL) {
        for (b2ContactEdge *c = monsterBody->GetContactList();c;c = c->next){
            b2Contact *contact = c->contact;
            b2Body *bodyA = contact->GetFixtureA()->GetBody();
            b2Body *bodyB = contact->GetFixtureB()->GetBody();
            
            if (bodyA == this->brickBody || bodyB == this->brickBody) {
                if (contact->IsTouching() == true) {
                    
                    brickForceVec.x =monsterBody->GetPosition().x - this->brickBody->GetPosition().x;
                    brickForceVec.y =monsterBody->GetPosition().y - this->brickBody->GetPosition().y;
                    brickForceVec.Normalize();
                    
                    monsterBody->ApplyLinearImpulse(brickForceVec, monsterBody->GetWorldCenter(), true);
                    oldVec = b2Vec2(monsterBody->GetLinearVelocity().x, monsterBody->GetLinearVelocity().y);
                    oldVec.Normalize();
                    monsterBody->SetLinearVelocity(b2Vec2(oldVec.x * MONSTER_SPEED, oldVec.y * MONSTER_SPEED));
                    
                    
                    CCLOG("==collided!==");
                    
                    
                    switch (this->getLifePoint()) {
                        case 0:
                            if (this->brick->getName() == "brick") {
                                this->scheduleOnce(schedule_selector(BasicBrick::dealDeletion),0.03f);
                            }
                            break;
                            
                        case 1:{
                            this->setLifePoint(this->getLifePoint()+1);
                            //record the position
                            m_pos2_X = this->brick->getPosition().x;
                            m_pos2_Y = this->brick->getPosition().y;
                            //delete the old one
                            this->scheduleOnce(schedule_selector(BasicBrick::dealDeletion),0.03f);
                            this->scheduleOnce(schedule_selector(BasicBrick::dealRebirth_2),0.15f);

                            
                            break;}
                            
                        case 2:{
                            this->setLifePoint(this->getLifePoint()+1);
                            m_pos3_X = this->brick->getPosition().x;
                            m_pos3_Y = this->brick->getPosition().y;
                            
                            this->scheduleOnce(schedule_selector(BasicBrick::dealDeletion),0.03f);
                            
                            this->scheduleOnce(schedule_selector(BasicBrick::dealRebirth_3),0.15f);

                            
                            break;}
                            
                        case 3:
                            this->setLifePoint(this->getLifePoint()+1);
                            this->scheduleOnce(schedule_selector(BasicBrick::dealDeletion),0.03f);
                            break;
                            
                            
                        default:
                            //this->scheduleOnce(schedule_selector(BasicBrick::dealDeletion),0.03f);
                            break;
                    }
                    
                    
                    if (this->getAttr() == "contra") {
                        
                        ContraSingleton* st = ContraSingleton::GetInstance();
                        
                        if (st->getContraState() == false) {
                            st->setContraState(true);
                            st->setSchedule();
                            
                            //scheduleOnce(schedule_selector(ContraWithOutBossLayer::aFewSecondsSchedule), 5.0f);
                            
                        }else if (st->getContraState() == true){
                            st->setContraState(false);
                            
                            
                            st->stopSchedule();
                            
                            st->setContraState(true);
                            st->setSchedule();
                            

                        }
                        
                    }
                    
                    
                }
            }
            
            
            
        }
    }
    
    
}

void BasicBrick::dealRebirth_2(float dt){
    CCLOG("==rebirth_2 !!==");
    
    if (this->brick == NULL && this->brickBody == NULL ) {
        //add a new one at the previous place
        this->init(m_pos2_X, m_pos2_Y, "growingBrick2.png", "brick", this->getLifePoint());
    }
    
}

void BasicBrick::dealRebirth_3(float dt){
    CCLOG("==rebirth_3 !!==");
    if (this->brick == NULL && this->brickBody == NULL) {
        this->init(m_pos3_X, m_pos3_Y, "growingBrick3.png", "brick", this->getLifePoint());
    }
}

void BasicBrick::dealDeletion(float dt){
    CCLOG("==deleted!==");
    
    if (this->brickBody != NULL) {
        world->DestroyBody(this->brickBody);
        this->brickBody = NULL;
    }
    
    if (isSpiral) {
        this->unschedule(schedule_selector(BasicBrick::spiralPosCallback));
    }
    
    if (this->centralNode != NULL) {
        this->unschedule(schedule_selector(BasicBrick::posChangingCallback));
        
        //noisy bricks' situation
        if (this->brick != NULL) {
            this->centralNode->removeChild(this->brick);
            this->brick = NULL;
        }
        layer->removeChild(centralNode);
        this->centralNode = NULL;
        
    }else{
        
        //common situations
        if (this->brick != NULL) {
            layer->removeChild(brick);
            this->brick = NULL;
        }
        
        if (this->brick_cry != NULL) {
            layer->removeChild(brick_cry);
            this->brick_cry = NULL;
        }

    
    }
    
}







