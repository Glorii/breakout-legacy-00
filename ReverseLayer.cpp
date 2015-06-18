//
//  ReverseLayer.cpp
//  RebornOfBall
//
//  Created by LiuGloria on 15/5/8.
//
//

#include "ReverseLayer.h"

USING_NS_CC;


ReverseLayer::ReverseLayer(){
}

ReverseLayer::~ReverseLayer(){
}

bool ReverseLayer::init(){
    if(!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    brickGroupNode = Node::create();
    brickGroupNode->setPosition(Vec2(visibleSize.width /2, visibleSize.height /2));
    brickGroupNode->setRotation(0.0f);
    this->addChild(brickGroupNode, -1);
    
    auto sprite = Sprite::create("bg_reverse.jpg");
    sprite->setScale(1.042f);//  1334px / 1280px
    
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    this->addChild(sprite,-2);
    //sprite->setOpacity(127.5f);
    
    
    initGame();
    
    return true;
}

void ReverseLayer::initGame(){
    BackgroundLayer::initGame();
    
    
    rotate_count = 0;
    rotate_switch = false;
    
    disc = Sprite::create("disc.png");
    brickGroupNode->addChild(disc,-1);
    disc->setPosition(0,0);
    
    float totalBrickWidth = BRICK_COLUMN * BRICK_LENGTH + (BRICK_COLUMN - 1) * BRICK_GAP;
    addHorizontalBaffles(-totalBrickWidth /2 - BAFFLE_WIDTH /2,   totalBrickWidth /2 - BAFFLE_HEIGHT /2);
    addHorizontalBaffles(-totalBrickWidth /2 - BAFFLE_WIDTH /2, -(totalBrickWidth /2 - BAFFLE_HEIGHT /2));
    addHorizontalBaffles( totalBrickWidth /2 + BAFFLE_WIDTH /2,   totalBrickWidth /2 - BAFFLE_HEIGHT /2);
    addHorizontalBaffles( totalBrickWidth /2 + BAFFLE_WIDTH /2, -(totalBrickWidth /2 - BAFFLE_HEIGHT /2));
    
    addVerticalBaffles(-totalBrickWidth /2 + BAFFLE_HEIGHT /2,   totalBrickWidth /2 + BAFFLE_WIDTH /2);
    addVerticalBaffles( totalBrickWidth /2 - BAFFLE_HEIGHT /2,   totalBrickWidth /2 + BAFFLE_WIDTH /2);
    addVerticalBaffles(-totalBrickWidth /2 + BAFFLE_HEIGHT /2, -(totalBrickWidth /2 + BAFFLE_WIDTH /2));
    addVerticalBaffles( totalBrickWidth /2 - BAFFLE_HEIGHT /2, -(totalBrickWidth /2 + BAFFLE_WIDTH /2));
}

void ReverseLayer::initMonsterBody(){
    if (monster)
    {
        removeChild(monster);
    }
    monster  = NULL;
    
    //monster = rand()%2 == 0 ? Sprite::create("whiteBall.png") : Sprite::create("blackBall.png");
    monster = Sprite::create("ball_portal.png");
    Size monsterSize = monster->getContentSize();
    
    monster->setPosition(monsterBody->GetPosition().x * WORLDSCALE, monsterBody->GetPosition().y * WORLDSCALE);
    addChild(monster,-1);
    monsterBody->SetUserData(monster);
    
    
    float angle;
    switch (rand()%5)
    {
        case 0:angle = CC_DEGREES_TO_RADIANS(90 - rand()%30);break;
        case 1:
        case 2:angle = CC_DEGREES_TO_RADIANS(135 - rand()%45);break;
        default: angle = CC_DEGREES_TO_RADIANS(180 - rand()%45);break;
    }
    monsterBody->SetLinearVelocity(b2Vec2(MONSTER_SPEED * cos(angle),-MONSTER_SPEED * sin(angle)));
}

void ReverseLayer::addHorizontalBaffles(float pos_x, float pos_y){
    Sprite* baffle = Sprite::create("baffle.png");
    Size baffleSize = baffle->getContentSize();
    brickGroupNode->addChild(baffle,0);
    baffle->setPosition(pos_x,pos_y);
    //baffle->setRotation(90);
    
    b2BodyDef baffleBodyDef;
    baffleBodyDef.type = b2_staticBody;
    baffleBodyDef.position = b2Vec2((visibleSize.width /2 + pos_x) / WORLDSCALE, (visibleSize.height /2 + pos_y) / WORLDSCALE);
    //baffleBodyDef.angle = CC_DEGREES_TO_RADIANS(90);
    b2PolygonShape baffleShape;
    baffleShape.SetAsBox( baffleSize.width/2 / WORLDSCALE, baffleSize.height/2 / WORLDSCALE);
    
    b2FixtureDef baffleFixDef;
    baffleFixDef.shape = &baffleShape;
    baffleFixDef.density = 10.0f;
    baffleFixDef.friction = 0.3f;
    baffleFixDef.restitution = 0.2f;
    
    b2Body *baffleBody = world->CreateBody(&baffleBodyDef);
    baffleBody->CreateFixture(&baffleFixDef);
    baffleBody->SetUserData(baffle);
    baffle->setName("baffle");
    
    //Vec2 position = brickGroupNode->convertToWorldSpace(baffle->getPosition());
    //baffleBody->SetTransform(b2Vec2(position.x,position.y),CC_DEGREES_TO_RADIANS(brickGroupNode->getRotation()) );

}

void ReverseLayer::addVerticalBaffles(float pos_x, float pos_y){
    Sprite* baffle = Sprite::create("baffle.png");
    Size baffleSize = baffle->getContentSize();
    brickGroupNode->addChild(baffle,0);
    baffle->setPosition(pos_x,pos_y);
    baffle->setRotation(90);
    
    b2BodyDef baffleBodyDef;
    baffleBodyDef.type = b2_staticBody;
    baffleBodyDef.position = b2Vec2((visibleSize.width /2 + pos_x) / WORLDSCALE, (visibleSize.height /2 + pos_y) / WORLDSCALE);
    baffleBodyDef.angle = CC_DEGREES_TO_RADIANS(90);
    b2PolygonShape baffleShape;
    baffleShape.SetAsBox( baffleSize.height/2 / WORLDSCALE, baffleSize.width/2 / WORLDSCALE);
    
    b2FixtureDef baffleFixDef;
    baffleFixDef.shape = &baffleShape;
    baffleFixDef.density = 10.0f;
    baffleFixDef.friction = 0.3f;
    baffleFixDef.restitution = 0.2f;
    
    b2Body *baffleBody = world->CreateBody(&baffleBodyDef);
    baffleBody->CreateFixture(&baffleFixDef);
    baffleBody->SetUserData(baffle);
    baffle->setName("baffle");
}

void ReverseLayer::addBrick(int i, int j){
    Sprite *brick = Sprite::create("brick_square.png");
    Size brickSize = brick->getContentSize();
    
    b2BodyDef brickBodyDef;
    brickBodyDef.type = b2_staticBody;
    
    float disX = visibleSize.width  /2 - (BRICK_COLUMN * BRICK_LENGTH + (BRICK_COLUMN - 1) * BRICK_GAP) /2 ;
    float disY = visibleSize.height /2 - (BRICK_COLUMN * BRICK_LENGTH + (BRICK_COLUMN - 1) * BRICK_GAP) /2 ;
    
    brickBodyDef.position = b2Vec2( (disX + i * (BRICK_LENGTH + BRICK_GAP) + BRICK_LENGTH /2) / WORLDSCALE,
                                    (disY + j * (BRICK_LENGTH + BRICK_GAP) + BRICK_LENGTH /2) / WORLDSCALE);
    b2Body *brickBody = world->CreateBody(&brickBodyDef);
    
    //brick->setPosition(brickBodyDef.position.x * WORLDSCALE, brickBodyDef.position.y * WORLDSCALE);
    //addChild(brick,-1);
    //put them into a group node
    brickGroupNode->addChild(brick,0);
    
    brick->setPosition(brickGroupNode->convertToNodeSpace(Vec2(disX + i * (BRICK_LENGTH + BRICK_GAP) + BRICK_LENGTH /2,
                                                               disY + j * (BRICK_LENGTH + BRICK_GAP) + BRICK_LENGTH /2)));
    
    brickBody->SetUserData(brick);
    
    brick->setName("brick");
    
    b2PolygonShape brickShape;
    brickShape.SetAsBox(brickSize.width / 2 / WORLDSCALE, brickSize.height / 2 / WORLDSCALE);
    b2FixtureDef brickFixDef;
    brickFixDef.shape = &brickShape;
    brickFixDef.density = 1.0f;
    brickFixDef.friction = 0.3f;
    brickFixDef.restitution = 0.2f;
    brickBody->CreateFixture(&brickFixDef);
    //Vec2 position = brickGroupNode->convertToWorldSpace(brick->getPosition());
    //brickBody->SetTransform(b2Vec2(position.x,position.y),CC_DEGREES_TO_RADIANS(brickGroupNode->getRotation()) );
    
}



void ReverseLayer::addPaddle(){
    
    m_vec2_1 = new Vector2((float)(visibleSize.width/2), (float)(visibleSize.height/2));
    m_vec2_2 = new Vector2((float)(visibleSize.width/2), (float)(visibleSize.height/2));
    
    t_bInit = false;
    m_bMouseDown = false;
    m_nAngle = 0;
    
    //////////////////////////////////////////////new paddle body//////////////////////////////////////
    paddleNode = Node::create();
    paddleNode->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    paddle = Sprite::create("reverse_center.png");
    paddleNode->addChild(paddle);
    paddle->setPosition(- CIRCLE_RADIUS,0 );
    paddle->setRotation(90);
    this->addChild(paddleNode,-1);
    
    b2BodyDef paddleBodyDef;
    paddleBodyDef.type = b2_staticBody;
    paddleBodyDef.position = b2Vec2((visibleSize.width / 2 - CIRCLE_RADIUS)/ WORLDSCALE, (visibleSize.height / 2) / WORLDSCALE);
    paddleBodyDef.angle = CC_DEGREES_TO_RADIANS(90);
    b2PolygonShape paddleShape;
    paddleShape.SetAsBox( PADDLE_THICKNESS/2 / WORLDSCALE,70.f /2 / WORLDSCALE);//160.0f
    //paddleShape.TestPoint()
    b2FixtureDef paddleFixDef;
    paddleFixDef.shape = &paddleShape;
    paddleFixDef.density = 10.0f;
    paddleFixDef.friction = 0.3f;
    paddleFixDef.restitution = 0.2f;
    paddleBody = world->CreateBody(&paddleBodyDef);
    paddleBody->CreateFixture(&paddleFixDef);
    Vec2 position = paddleNode->convertToWorldSpace(paddle->getPosition());
    paddleBody->SetTransform(b2Vec2(position.x,position.y),CC_DEGREES_TO_RADIANS(paddleNode->getRotation()) );
    
    
    
    
    leftWing = Sprite::create("reverse_l_wing.png");
    paddleNode->addChild(leftWing);
    leftWing->setPosition(- CIRCLE_RADIUS,35 );//70
    leftWing->setRotation(90);
    b2BodyDef leftWingBodyDef;
    
    //1.set type for wallBrick:kinematic
    leftWingBodyDef.type = b2_kinematicBody;
    //2.set up shape:polygon
    b2PolygonShape leftWingShape;
    /*b2Vec2 vertices[4] = {
     b2Vec2((visibleSize.width/2 -CIRCLE_RADIUS-PADDLE_THICKNESS)/ WORLDSCALE, (visibleSize.height/2 + 60)/ WORLDSCALE),
     b2Vec2((visibleSize.width/2 -CIRCLE_RADIUS)/ WORLDSCALE, (visibleSize.height/2 + 140)/ WORLDSCALE),
     b2Vec2((visibleSize.width/2 -CIRCLE_RADIUS)/ WORLDSCALE, (visibleSize.height/2)/ WORLDSCALE),
     b2Vec2((visibleSize.width/2 -CIRCLE_RADIUS-PADDLE_THICKNESS)/ WORLDSCALE, (visibleSize.height/2) / WORLDSCALE)
     };*/
    /*b2Vec2 vertices[4] = {
        b2Vec2(-40/ WORLDSCALE, -10/ WORLDSCALE),
        b2Vec2( 40/ WORLDSCALE,  70/ WORLDSCALE),
        b2Vec2(-40/ WORLDSCALE, -70/ WORLDSCALE),
        b2Vec2( 40/ WORLDSCALE, -70/ WORLDSCALE)
    };*/
    b2Vec2 vertices[4] = {
        b2Vec2(-20/ WORLDSCALE,   0/ WORLDSCALE),
        b2Vec2( 20/ WORLDSCALE,  35/ WORLDSCALE),
        b2Vec2( 20/ WORLDSCALE, -35/ WORLDSCALE),
        b2Vec2(-20/ WORLDSCALE, -35/ WORLDSCALE)
    };
    
    leftWingShape.Set(vertices,4);
    //3.set up fixture definition
    b2FixtureDef leftWingFixDef;
    leftWingFixDef.shape = &leftWingShape;
    leftWingFixDef.density = 10.0f;
    leftWingFixDef.friction = 0.3f;
    leftWingFixDef.restitution = 0.2f;
    //4.set up wallBrick body using fixture
    leftWingBody  =world->CreateBody(&leftWingBodyDef);
    leftWingBody->CreateFixture(&leftWingFixDef);
    Vec2 leftWingPosition = paddleNode->convertToWorldSpace(leftWing->getPosition());
    leftWingBody->SetTransform(b2Vec2(leftWingPosition.x,leftWingPosition.y),CC_DEGREES_TO_RADIANS(paddleNode->getRotation()) );
    
    
    rightWing = Sprite::create("reverse_r_wing.png");
    paddleNode->addChild(rightWing);
    rightWing->setPosition(- CIRCLE_RADIUS,-35 );//-70
    rightWing->setRotation(90);
    b2BodyDef rightWingBodyDef;
    
    //1.set type for wallBrick:kinematic
    rightWingBodyDef.type = b2_kinematicBody;
    //2.set up shape:polygon
    b2PolygonShape rightWingShape;
    /*b2Vec2 r_vertices[4] = {
        b2Vec2(-40/ WORLDSCALE,  70/ WORLDSCALE),
        b2Vec2( 40/ WORLDSCALE,  70/ WORLDSCALE),
        b2Vec2( 40/ WORLDSCALE, -70/ WORLDSCALE),
        b2Vec2(-40/ WORLDSCALE,  10/ WORLDSCALE)
    };*/
    b2Vec2 r_vertices[4] = {
        b2Vec2(-20/ WORLDSCALE,  35/ WORLDSCALE),
        b2Vec2( 20/ WORLDSCALE,  35/ WORLDSCALE),
        b2Vec2( 20/ WORLDSCALE, -35/ WORLDSCALE),
        b2Vec2(-20/ WORLDSCALE,   0/ WORLDSCALE)
    };
    
    rightWingShape.Set(r_vertices,4);
    //3.set up fixture definition
    b2FixtureDef rightWingFixDef;
    rightWingFixDef.shape = &rightWingShape;
    rightWingFixDef.density = 10.0f;
    rightWingFixDef.friction = 0.3f;
    rightWingFixDef.restitution = 0.2f;
    //4.set up wallBrick body using fixture
    rightWingBody  =world->CreateBody(&rightWingBodyDef);
    rightWingBody->CreateFixture(&rightWingFixDef);
    Vec2 rightWingPosition = paddleNode->convertToWorldSpace(rightWing->getPosition());
    rightWingBody->SetTransform(b2Vec2(rightWingPosition.x,rightWingPosition.y),CC_DEGREES_TO_RADIANS(paddleNode->getRotation()) );
    
}


void ReverseLayer::update(float dur){
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
        && monster->getPosition().distance(paddleNode->getPosition()) < CIRCLE_RADIUS){
        
        drawNode->drawSegment(Vec2(monster->getPosition().x, monster->getPosition().y), Vec2(m_x, m_y), 3.0f, Color4F(1, 1, 1, 1));
        drawNode->drawSegment(Vec2(m_x, m_y), Vec2(mReflectX, mReflectY), 3.0f, Color4F(1, 1, 1, 1));
    }
    
    //apply the gravity which is always pointing to the centre
    //monsterBody->ApplyForceToCenter(b2Vec2(-dirVec.x * 10000 / WORLDSCALE, -dirVec.y * 10000 / WORLDSCALE), true);
    //monsterBody->ApplyForce(b2Vec2(-dirVec.x * 10000 / WORLDSCALE, -dirVec.y * 10000 / WORLDSCALE),
    //                        b2Vec2(paddleNode->getPosition().x / WORLDSCALE, paddleNode->getPosition().y / WORLDSCALE), true);
    monsterBody->ApplyLinearImpulse(b2Vec2(-dirVec.x / 20 / WORLDSCALE, -dirVec.y / 20 / WORLDSCALE), monsterBody->GetWorldCenter(), true);
    
    
    oldVec = b2Vec2(monsterBody->GetLinearVelocity().x, monsterBody->GetLinearVelocity().y);
    oldVec.Normalize();
    monsterBody->SetLinearVelocity(b2Vec2(oldVec.x * MONSTER_SPEED, oldVec.y * MONSTER_SPEED));
    
    
    rotate_count++;
    if (rotate_count > 180) {
        rotate_count = 0;
        if (rand() % 2 == 0) {
            rotate_switch = false;
            brickGroupNode->setRotation(brickGroupNode->getRotation() + 0.5f);
        }else{
            rotate_switch = true;
            brickGroupNode->setRotation(brickGroupNode->getRotation() - 0.5f);
        }
    }else{
        if (rotate_switch == false) {
            brickGroupNode->setRotation(brickGroupNode->getRotation() + 0.5f);
        }else if(rotate_switch == true){
            brickGroupNode->setRotation(brickGroupNode->getRotation() - 0.5f);
        }
    }
    
    //brickGroupNode->setRotation(45);
    b_group_angle = -( CC_DEGREES_TO_RADIANS(brickGroupNode->getRotation()) );
    
    
    for (b2Body *body = world->GetBodyList(); body!=NULL; body = body->GetNext()) {
        if ((Sprite*)body->GetUserData() != NULL) {
            if (((Sprite*)body->GetUserData())->getName() == "baffle" ||
                ((Sprite*)body->GetUserData())->getName() == "brick") {
                m_sprite = (Sprite*)body->GetUserData();
                
                Vec2 m_pos = brickGroupNode->convertToWorldSpace( m_sprite->getPosition());
                b2Vec2 m_b_position = b2Vec2(m_pos.x / WORLDSCALE, m_pos.y / WORLDSCALE);
                body->SetTransform(m_b_position,b_group_angle);
            }
            
        }
    }
    
    this->dealCollision(dur);
    
    world->ClearForces();
    world->DrawDebugData();
}

void ReverseLayer::dealCollision(float dur){
    for (b2ContactEdge *c = monsterBody->GetContactList();c;c = c->next){
        b2Contact *contact = c->contact;
        b2Body *bodyA = contact->GetFixtureA()->GetBody();
        b2Body *bodyB = contact->GetFixtureB()->GetBody();
        
        
        b2Body *collidedBody = (bodyA == monsterBody)?bodyB:bodyA;
        Sprite* sp = (Sprite*)collidedBody->GetUserData();
        
        if ((collidedBody == paddleBody/* || sp->getName() == "leftWing" || sp->getName() == "rightWing"*/) && contact->IsTouching() == true)
        {
            //records the direction from the center point to the monster
            Vec2 vec1 = paddleNode->convertToWorldSpace( Vec2(paddle->getBoundingBox().getMinX(),paddle->getBoundingBox().getMaxY()) );
            Vec2 vec2 = paddleNode->convertToWorldSpace( Vec2(paddle->getBoundingBox().getMaxX(),paddle->getBoundingBox().getMaxY()) );
            
            
            Vec2 normalVec = vec1 - vec2;
            Vec2 monsterToCentreVec = paddleNode->getPosition() - monster->getPosition();
            //if the normalVec is pointing to the centre
            dirVec = normalVec.dot(monsterToCentreVec) > 0? normalVec:normalVec*(-1);
            
            dirVec.normalize();
            float m_velocity = sqrtf(monsterBody->GetLinearVelocity().x * monsterBody->GetLinearVelocity().x +
                                     monsterBody->GetLinearVelocity().y * monsterBody->GetLinearVelocity().y);
            
            forceVec.x = dirVec.x * 2 * monsterBody->GetMass() * m_velocity;
            forceVec.y = dirVec.y * 2 * monsterBody->GetMass() * m_velocity;
            
            //forceVec.Normalize();
            
            monsterBody->ApplyLinearImpulse(forceVec, monsterBody->GetWorldCenter(), true);
            oldVec = b2Vec2(monsterBody->GetLinearVelocity().x, monsterBody->GetLinearVelocity().y);
            oldVec.Normalize();
            monsterBody->SetLinearVelocity(b2Vec2(oldVec.x * MONSTER_SPEED, oldVec.y * MONSTER_SPEED));
            return;
        }
        
        if ((collidedBody == leftWingBody) && contact->IsTouching() == true)
        {
            //records the direction from the center point to the monster
            Vec2 vec1 = paddleNode->convertToWorldSpace( Vec2(leftWing->getBoundingBox().getMinX(),leftWing->getBoundingBox().getMaxY()) );
            Vec2 vec2 = paddleNode->convertToWorldSpace( Vec2(leftWing->getBoundingBox().getMaxX(),leftWing->getBoundingBox().getMaxY()) );
            
            
            Vec2 normalVec = vec1 - vec2;
            Vec2 monsterToCentreVec = paddleNode->getPosition() - monster->getPosition();
            //if the normalVec is pointing to the centre
            dirVec = normalVec.dot(monsterToCentreVec) > 0? normalVec:normalVec*(-1);
            
            dirVec.normalize();
            float m_velocity = sqrtf(monsterBody->GetLinearVelocity().x * monsterBody->GetLinearVelocity().x +
                                     monsterBody->GetLinearVelocity().y * monsterBody->GetLinearVelocity().y);
            
            forceVec.x = dirVec.x * 2 * monsterBody->GetMass() * m_velocity;
            forceVec.y = dirVec.y * 2 * monsterBody->GetMass() * m_velocity;
            
            //forceVec.Normalize();
            
            monsterBody->ApplyLinearImpulse(forceVec, monsterBody->GetWorldCenter(), true);
            oldVec = b2Vec2(monsterBody->GetLinearVelocity().x, monsterBody->GetLinearVelocity().y);
            oldVec.Normalize();
            monsterBody->SetLinearVelocity(b2Vec2(oldVec.x * MONSTER_SPEED, oldVec.y * MONSTER_SPEED));
            return;
        }
        
        if ((collidedBody == rightWingBody) && contact->IsTouching() == true)
        {
            //records the direction from the center point to the monster
            Vec2 vec1 = paddleNode->convertToWorldSpace( Vec2(rightWing->getBoundingBox().getMinX(),rightWing->getBoundingBox().getMaxY()) );
            Vec2 vec2 = paddleNode->convertToWorldSpace( Vec2(rightWing->getBoundingBox().getMaxX(),rightWing->getBoundingBox().getMaxY()) );
            
            
            Vec2 normalVec = vec1 - vec2;
            Vec2 monsterToCentreVec = paddleNode->getPosition() - monster->getPosition();
            //if the normalVec is pointing to the centre
            dirVec = normalVec.dot(monsterToCentreVec) > 0? normalVec:normalVec*(-1);
            
            dirVec.normalize();
            float m_velocity = sqrtf(monsterBody->GetLinearVelocity().x * monsterBody->GetLinearVelocity().x +
                                     monsterBody->GetLinearVelocity().y * monsterBody->GetLinearVelocity().y);
            
            forceVec.x = dirVec.x * 2 * monsterBody->GetMass() * m_velocity;
            forceVec.y = dirVec.y * 2 * monsterBody->GetMass() * m_velocity;
            
            //forceVec.Normalize();
            
            monsterBody->ApplyLinearImpulse(forceVec, monsterBody->GetWorldCenter(), true);
            oldVec = b2Vec2(monsterBody->GetLinearVelocity().x, monsterBody->GetLinearVelocity().y);
            oldVec.Normalize();
            monsterBody->SetLinearVelocity(b2Vec2(oldVec.x * MONSTER_SPEED, oldVec.y * MONSTER_SPEED));
            return;
        }

        
        
        if (sp && (sp->getName() == "brick" || sp->getName() == "wall" || sp->getName() == "baffle") && contact->IsTouching() == true) {
            
            brickForceVec.x =monsterBody->GetPosition().x - collidedBody->GetPosition().x;
            brickForceVec.y =monsterBody->GetPosition().y - collidedBody->GetPosition().y;
            brickForceVec.Normalize();
            monsterBody->ApplyLinearImpulse(brickForceVec, monsterBody->GetWorldCenter(), true);
            oldVec = b2Vec2(monsterBody->GetLinearVelocity().x, monsterBody->GetLinearVelocity().y);
            oldVec.Normalize();
            monsterBody->SetLinearVelocity(b2Vec2(oldVec.x * MONSTER_SPEED, oldVec.y * MONSTER_SPEED));
            
            if (sp->getName() == "brick")
            {
                for (int i = 0; collidedBodies[i] == NULL; i++) {
                    collidedBodies[i] = collidedBody;
                    break;
                }
                this->scheduleOnce(schedule_selector(ReverseLayer::dealDeletion),0.05f);
            }
            
        }
    }
    
}

void ReverseLayer::dealDeletion(float dt){
    //CCLOG("call dealDeletion once");
    b2Body *bodyToDelete;
    Sprite* sp;
    
    for (int i=0; i<COLLIDED_NUMBER; i++) {
        if (collidedBodies[i]) {
            bodyToDelete = collidedBodies[i];
            if (bodyToDelete->GetUserData()) {
                sp = (Sprite*)bodyToDelete->GetUserData();
                
                world->DestroyBody(bodyToDelete);
                brickGroupNode->removeChild(sp);
            }
        }
        
    }
    for (int i=0; i<COLLIDED_NUMBER; i++) {
        if (collidedBodies[i]) {
            collidedBodies[i] = NULL;
        }
        
    }
}


void ReverseLayer::onTouchMoved( Touch* touch, Event *event)
{
    auto location = touch->getLocation();
    
    if(!m_bMouseDown){
        return;
    }
    
    m_vec2_2->updatefunc(location.x ,location.y);
    m_nAngle = Vector2::Angle(*m_vec2_1 , *m_vec2_2);//degree
    m_vec2_1->updatefunc(location.x ,location.y);
    
    
    float p_rotation = paddleNode->getRotation();
    paddleNode->setRotation(p_rotation + m_nAngle);
    
    
    //paddle->setPosition(Vec2(paddle->getPosition().x + 0.3f, paddle->getPosition().y));
    
    Vec2 position = paddleNode->convertToWorldSpace(paddle->getPosition());
    b2Vec2 p_position = b2Vec2(position.x / WORLDSCALE, position.y / WORLDSCALE);
    float p_angle = -( CC_DEGREES_TO_RADIANS(paddleNode->getRotation()) );
    paddleBody->SetTransform(p_position,p_angle);
    
    if (rotate_switch == false) {
        if (m_nAngle > 0) {
            if (leftWing->getPosition().y <= 150) {
                leftWing->setPosition(Vec2(leftWing->getPosition().x, leftWing->getPosition().y + 2.0f));
                rightWing->setPosition(Vec2(rightWing->getPosition().x, rightWing->getPosition().y - 2.0f));
            }
        }else if(m_nAngle < 0){
            if (leftWing->getPosition().y >= 30) {
                leftWing->setPosition(Vec2(leftWing->getPosition().x, leftWing->getPosition().y - 2.0f));
                rightWing->setPosition(Vec2(rightWing->getPosition().x, rightWing->getPosition().y + 2.0f));
            }
        }

    }else if (rotate_switch == true){
        if (m_nAngle < 0) {
            if (leftWing->getPosition().y <= 150) {
                leftWing->setPosition(Vec2(leftWing->getPosition().x, leftWing->getPosition().y + 2.0f));
                rightWing->setPosition(Vec2(rightWing->getPosition().x, rightWing->getPosition().y - 2.0f));
            }
        }else if(m_nAngle > 0){
            if (leftWing->getPosition().y >= 30) {
                leftWing->setPosition(Vec2(leftWing->getPosition().x, leftWing->getPosition().y - 2.0f));
                rightWing->setPosition(Vec2(rightWing->getPosition().x, rightWing->getPosition().y + 2.0f));
            }
        }

    }
    
    
    Vec2 leftWingPos = paddleNode->convertToWorldSpace(leftWing->getPosition());
    b2Vec2 l_position = b2Vec2(leftWingPos.x / WORLDSCALE, leftWingPos.y / WORLDSCALE);
    leftWingBody->SetTransform(l_position,p_angle);
    
    
    Vec2 rightWingPos = paddleNode->convertToWorldSpace(rightWing->getPosition());
    b2Vec2 r_position = b2Vec2(rightWingPos.x / WORLDSCALE, rightWingPos.y / WORLDSCALE);
    rightWingBody->SetTransform(r_position,p_angle);
    
    
    m_nAngle = 0.0;
    
}

void ReverseLayer::initBrickGroup()
{
    for (int i = 0; i < BRICK_COLUMN; i++) {
        for (int j = 0; j < BRICK_COLUMN; j++) {
            addBrick(i, j);
            
            
        }
    }
}