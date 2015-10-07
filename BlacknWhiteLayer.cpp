//
//  BlacknWhiteLayer.cpp
//  helloworld
//
//  Created by LiuGloria on 15/4/17.
//
//

#include "BlacknWhiteLayer.h"

USING_NS_CC;



BlacknWhiteLayer::BlacknWhiteLayer(){
    mReflectX_Black = 0;
    mReflectY_Black = 0;
    
    m_x_Black = 0;
    m_y_Black = 0;
}

BlacknWhiteLayer::~BlacknWhiteLayer(){
}

void BlacknWhiteLayer::addPaddle(){
    m_vec2_1 = new Vector2((float)(visibleSize.width/2), (float)(visibleSize.height/2));
    m_vec2_2 = new Vector2((float)(visibleSize.width/2), (float)(visibleSize.height/2));
    
    t_bInit = false;
    m_bMouseDown = false;
    m_nAngle = 0;
    
    //////////////////////////////////////////////new paddle body//////////////////////////////////////
    paddleNode = Node::create();
    paddleNode->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    paddle = Sprite::create("paddle.png");
    paddleNode->addChild(paddle);
    paddle->setPosition(-357,0 );
    paddle->setRotation(90);
    this->addChild(paddleNode);
    
    b2BodyDef paddleBodyDef;
    paddleBodyDef.type = b2_staticBody;
    paddleBodyDef.position = b2Vec2((visibleSize.width / 2 - CIRCLE_RADIUS)/ WORLDSCALE, (visibleSize.height / 2) / WORLDSCALE);
    paddleBodyDef.angle = CC_DEGREES_TO_RADIANS(90);
    b2PolygonShape paddleShape;
    paddleShape.SetAsBox( 80/2 / WORLDSCALE,440/2 / WORLDSCALE);
    //paddleShape.TestPoint()
    b2FixtureDef paddleFixDef;
    paddleFixDef.shape = &paddleShape;
    paddleFixDef.density = 10.0f;
    paddleFixDef.friction = 0.3;
    paddleFixDef.restitution = 0.2f;
    paddleBody = world->CreateBody(&paddleBodyDef);
    paddleBody->CreateFixture(&paddleFixDef);
    Vec2 position = paddleNode->convertToWorldSpace(paddle->getPosition());
    paddleBody->SetTransform(b2Vec2(position.x,position.y),CC_DEGREES_TO_RADIANS(paddleNode->getRotation()) );

    
    //the opposite paddle
    paddle2 = Sprite::create("paddle2.png");
    paddleNode->addChild(paddle2);
    paddle2->setPosition(357,0);
    paddle2->setRotation(90);

    b2BodyDef paddleBodyDef2;
    paddleBodyDef2.type = b2_staticBody;
    //(2a-m,2b-n)
    //paddleBodyDef2.position = b2Vec2( (2 * paddleNode->getPosition().x -(visibleSize.width / 2 - CIRCLE_RADIUS))/ WORLDSCALE,
    //                                  (2 * paddleNode->getPosition().y -(visibleSize.height / 2)) / WORLDSCALE);
    paddleBodyDef2.position = b2Vec2((visibleSize.width / 2 + CIRCLE_RADIUS)/ WORLDSCALE, (visibleSize.height / 2) / WORLDSCALE);
    paddleBodyDef2.angle = CC_DEGREES_TO_RADIANS(90);
    b2PolygonShape paddleShape2;
    paddleShape2.SetAsBox( 80/2 / WORLDSCALE,440/2 / WORLDSCALE);
    
    b2FixtureDef paddleFixDef2;
    paddleFixDef2.shape = &paddleShape2;
    paddleFixDef2.density = 10.0f;
    paddleFixDef2.friction = 0.3;
    paddleFixDef2.restitution = 0.2f;
    paddleBody2 = world->CreateBody(&paddleBodyDef2);
    paddleBody2->CreateFixture(&paddleFixDef2);
    Vec2 position2 = paddleNode->convertToWorldSpace(paddle2->getPosition());
    paddleBody2->SetTransform(b2Vec2(position2.x,position2.y),CC_DEGREES_TO_RADIANS(paddleNode->getRotation()) );
    
    printf("position of paddlebody2 is %f,%f\n",paddleBody2->GetPosition().x,paddleBody2->GetPosition().y);
    printf("opposite paddle added!\n");
    
}

void BlacknWhiteLayer::addMonsterBody(){
    b2BodyDef monsterBodyDef;
    monsterBodyDef.type = b2_dynamicBody;
    monsterBodyDef.position = b2Vec2(460 / WORLDSCALE, 280 / WORLDSCALE);
    
    
    b2CircleShape monsterShape;
    monsterShape.m_radius = MONSTER_RADIUS / WORLDSCALE;
    
    b2FixtureDef monsterFixDef;
    monsterFixDef.shape = &monsterShape;
    monsterFixDef.density = 1.0f;
    monsterFixDef.friction = MONSTER_FRICTION;
    monsterFixDef.restitution = 0.0f;
    
    monsterBody = world->CreateBody(&monsterBodyDef);
    monsterBody->CreateFixture(&monsterFixDef);
    
    //monster = Sprite::create("ball.png");
    //Size monsterSize = monster->getContentSize();
    
    //monster->setPosition(monsterBody->GetPosition().x * WORLDSCALE, monsterBody->GetPosition().y * WORLDSCALE);
    //addChild(monster);
    //monsterBody->SetUserData(monster);
    
    
    
    /*b2BodyDef monsterBodyDef;
     monsterBodyDef.type = b2_dynamicBody;
     monsterBodyDef.position = b2Vec2(572 / WORLDSCALE, 280 / WORLDSCALE);
     
     
     b2CircleShape monsterShape;
     monsterShape.m_radius = MONSTER_RADIUS / WORLDSCALE;
     
     b2FixtureDef monsterFixDef;
     monsterFixDef.shape = &monsterShape;
     monsterFixDef.density = 1.0f;
     monsterFixDef.friction = MONSTER_FRICTION;
     monsterFixDef.restitution = 0.0f;
     
     monsterBody = world->CreateBody(&monsterBodyDef);
     
     
     monster = Sprite::create("ball.png");
     Size monsterSize = monster->getContentSize();
     
     monster->setPosition(monsterBodyDef.position.x * WORLDSCALE, monsterBodyDef.position.y * WORLDSCALE);
     addChild(monster);
     monsterBody->SetUserData(monster);
     
     monsterBody->CreateFixture(&monsterFixDef);*/
}

void BlacknWhiteLayer::initMonsterBody(){
    if(monster)
    {
        removeChild(monster);
        
    }
    monster  = NULL;
    
    //monster = rand()%2 == 0 ? Sprite::create("whiteBall.png") : Sprite::create("blackBall.png");
    int m_color = rand()%2;
    if (m_color == 0) {
        monster = Sprite::create("whiteBall.png");
        monster->setName("whiteBall");
    }else if (m_color == 1){
        monster = Sprite::create("blackBall.png");
        monster->setName("blackBall");
    }
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
    monsterBody->SetLinearVelocity(b2Vec2(MONSTER_SPEED * cos(angle),-MONSTER_SPEED * sin(angle)));
    
    
    /*float angle;
     switch (rand()%5)
     {
     case 0:angle = CC_DEGREES_TO_RADIANS(90 - rand()%30);break;
     case 1:
     case 2:angle = CC_DEGREES_TO_RADIANS(135 - rand()%45);break;
     default: angle = CC_DEGREES_TO_RADIANS(180 - rand()%45);break;
     }
     monsterBody->SetLinearVelocity(b2Vec2(MONSTER_SPEED * cos(angle),-MONSTER_SPEED * sin(angle)));*/
}

void BlacknWhiteLayer::calInterPoint_Black(){
    Vec2 v1 = paddleNode->convertToWorldSpace( Vec2(paddle2->getBoundingBox().getMaxX(),paddle2->getBoundingBox().getMinY()) );
    Vec2 v2 = paddleNode->convertToWorldSpace( Vec2(paddle2->getBoundingBox().getMaxX(),paddle2->getBoundingBox().getMaxY()) );
    
    float x1 = v1.x;
    float y1 = v1.y;
    float x2 = v2.x;
    float y2 = v2.y;
    
    b2Vec2 v = monsterBody->GetLinearVelocity();
    float k = 0;
    //monster position
    float x0 = monster->getPosition().x;
    float y0 = monster->getPosition().y;
    
    
    //calculates distinctively if the ball is flying vertically
    if (v.x == 0) {
        m_x_Black = x0;
        if (x2 - x1 != 0) {
            m_y_Black = (y2 - y1) * (m_x_Black - x1) / (x2 - x1) + y1;
        }
        
    }
    if (v.x != 0) {
        k = v.y / v.x;
        if (y2 - y1 - k * (x2 - x1) != 0) {
            m_x_Black = ( (x2 - x1) * (y0 - y1 - k * x0) + x1 * y2 - x1 * y1) / (y2 - y1 - k * (x2 - x1));
            m_y_Black = k * (m_x_Black - x0) + y0;
        }
    }
    
}

void BlacknWhiteLayer::calReflectedRay_Black(){
    Vec2 vec1 = paddleNode->convertToWorldSpace( Vec2(paddle2->getBoundingBox().getMaxX(),paddle2->getBoundingBox().getMaxY()) );
    Vec2 vec2 = paddleNode->convertToWorldSpace( Vec2(paddle2->getBoundingBox().getMinX(),paddle2->getBoundingBox().getMaxY()) );
    
    vec1.subtract(vec2);
    Vec2 normalVec = vec1;
    normalVec.normalize();
    
    
    float x_1 = m_x_Black + 357 * normalVec.x;
    float y_1 = m_y_Black + 357 * normalVec.y;
    float x_2 = m_x_Black;
    float y_2 = m_y_Black;
    float x_0 = monster->getPosition().x;
    float y_0 = monster->getPosition().y;
    
    
    float A_ = y_2 - y_1;
    float B_ = x_1 - x_2;
    float C_ = x_2 * y_1 - x_1 * y_2;
    float k_ = -2 * (A_ * x_0 + B_ * y_0 + C_) / (A_ * A_ + B_ * B_);
    
    if (x_2 - x_1 != 0 && y_2 - y_1 != 0) {
        mReflectX_Black = x_0 + k_ * A_;
        mReflectY_Black = y_0 + k_ * B_;
    }
}


void BlacknWhiteLayer::update(float dur){
    int velIteration = 8;
    int postIteration = 3;
    world->Step(dur, velIteration, postIteration);
    
    //synchronizing its skin
    monster->setPosition(monsterBody->GetPosition().x * WORLDSCALE, monsterBody->GetPosition().y * WORLDSCALE);
    monster->setRotation(CC_RADIANS_TO_DEGREES(monsterBody->GetAngle()));
    
    
    //visualization
    calInterPoint();
    calReflectedRay();
    calInterPoint_Black();
    calReflectedRay_Black();
    
    drawNode->clear();
    Vec2 dirVec = Vec2(monster->getPosition().x - paddleNode->getPosition().x, monster->getPosition().y - paddleNode->getPosition().y);
    float multiNum = dirVec.x * monsterBody->GetLinearVelocity().x + dirVec.y * monsterBody->GetLinearVelocity().y;
    if (/*multiNum > 0 &&*/ monster->getPosition().distance(paddleNode->getPosition()) > MONSTER_RADIUS
        && monster->getPosition().distance(paddleNode->getPosition()) < CIRCLE_RADIUS)
    {
        //draw the white line
        //if (monster->getName() == "whiteBall") {
            drawNode->drawSegment(Vec2(monster->getPosition().x, monster->getPosition().y), Vec2(m_x, m_y), 3.0f, Color4F(1, 1, 1, 1));
            drawNode->drawSegment(Vec2(m_x, m_y), Vec2(mReflectX, mReflectY), 3.0f, Color4F(1, 1, 1, 1));
        //}else if (monster->getName() == "blackBall"){//draw the black line
            drawNode->drawSegment(Vec2(monster->getPosition().x, monster->getPosition().y), Vec2(m_x_Black, m_y_Black), 3.0f, Color4F(0.8, 0.8, 0.8, 1));
            drawNode->drawSegment(Vec2(m_x_Black, m_y_Black), Vec2(mReflectX_Black, mReflectY_Black), 3.0f, Color4F(1, 1, 1, 1));
        //}
    }
    
    
    
    /*String* monsterMass = String::createWithFormat("%f",monsterBody->GetMass());
     LabelTTF *label;
     label->setString(&monsterMass);
     label->setString(monsterBody->GetMass())
     LabelTTF *label = LabelTTF::labelWithString("hello world", CCSizeMake(100, 25), (CCTextAlignment)intAlign, strFontName, fontSize);
     label->setAnchorPoint(ccp(0, 0));
     label->setPosition = ccp(100,100);*/
    
    this->dealCollision(dur);
    
    for (int i = 0; i < BRICK_COUNT; i++) {
        bricks[i]->dealCollision(dur, monsterBody);
    }
    
    
    world->ClearForces();
    world->DrawDebugData();
}



void BlacknWhiteLayer::dealCollision(float dur){
    for (b2ContactEdge *c = monsterBody->GetContactList();c;c = c->next){
        b2Contact *contact = c->contact;
        b2Body *bodyA = contact->GetFixtureA()->GetBody();
        b2Body *bodyB = contact->GetFixtureB()->GetBody();
        
        
        b2Body *collidedBody = (bodyA == monsterBody)?bodyB:bodyA;
        
        if ((collidedBody == paddleBody || collidedBody == paddleBody2) && contact->IsTouching() == true)
        {
            if ((collidedBody == paddleBody && monster->getName() == "blackBall") || (collidedBody == paddleBody2 && monster->getName() == "whiteBall")) {
                //records the direction from the center point to the monster
                Vec2 vec1;
                Vec2 vec2;
                if (collidedBody == paddleBody) {
                    vec1 = paddleNode->convertToWorldSpace( Vec2(paddle->getBoundingBox().getMinX(),paddle->getBoundingBox().getMaxY()) );
                    vec2 = paddleNode->convertToWorldSpace( Vec2(paddle->getBoundingBox().getMaxX(),paddle->getBoundingBox().getMaxY()) );
                }
                if (collidedBody == paddleBody2) {
                    vec1 = paddleNode->convertToWorldSpace( Vec2(paddle2->getBoundingBox().getMinX(),paddle2->getBoundingBox().getMaxY()) );
                    vec2 = paddleNode->convertToWorldSpace( Vec2(paddle2->getBoundingBox().getMaxX(),paddle2->getBoundingBox().getMaxY()) );
                }
                
                
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
            }else{
                monsterBody->SetTransform(b2Vec2(460 / WORLDSCALE, 280 / WORLDSCALE), 0);
                initMonsterBody();
            }
        }
        
        /*Sprite* sp = (Sprite*)collidedBody->GetUserData();
        if (sp && sp->getName() == "brick" && contact->IsTouching() == true) {
            for (int i = 0; collidedBodies[i] == NULL; i++) {
                collidedBodies[i] = collidedBody;
                break;
            }
            
            brickForceVec.x =monsterBody->GetPosition().x - collidedBody->GetPosition().x;
            brickForceVec.y =monsterBody->GetPosition().y - collidedBody->GetPosition().y;
            brickForceVec.Normalize();
            monsterBody->ApplyLinearImpulse(brickForceVec, monsterBody->GetWorldCenter(), true);
            oldVec = b2Vec2(monsterBody->GetLinearVelocity().x, monsterBody->GetLinearVelocity().y);
            oldVec.Normalize();
            monsterBody->SetLinearVelocity(b2Vec2(oldVec.x * MONSTER_SPEED, oldVec.y * MONSTER_SPEED));
            
            this->scheduleOnce(schedule_selector(BlacknWhiteLayer::dealDeletion),0.05f);
        }*/
    }
    
}

bool BlacknWhiteLayer::onTouchBegan( Touch* touch, Event *event )
{
    monsterBody->SetTransform(b2Vec2(460 / WORLDSCALE, 280 / WORLDSCALE), 0);
    initMonsterBody();
    
    auto location = touch->getLocation();
    
    Vec2 relatedLocation = paddleNode->convertToNodeSpace(touch->getLocation());
    if(paddle->getBoundingBox().containsPoint(relatedLocation) || paddle2->getBoundingBox().containsPoint(relatedLocation))
    {
        if(!m_bMouseDown){
            m_bMouseDown=true;
            m_vec2_1->updatefunc(location.x ,location.y);
            m_nAngle=0;
        }
    }
    return true;
}



void BlacknWhiteLayer::onTouchMoved( Touch* touch, Event *event)
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
    
    Vec2 position = paddleNode->convertToWorldSpace(paddle->getPosition());
    b2Vec2 p_position = b2Vec2(position.x / WORLDSCALE, position.y / WORLDSCALE);
    float p_angle = -( CC_DEGREES_TO_RADIANS(paddleNode->getRotation()) );
    paddleBody->SetTransform(p_position,p_angle);
    
    
    Vec2 position2 = paddleNode->convertToWorldSpace(paddle2->getPosition());
    b2Vec2 p_position2 = b2Vec2(position2.x / WORLDSCALE, position2.y / WORLDSCALE);
    paddleBody2->SetTransform(p_position2, p_angle);
    
    m_nAngle = 0.0;
    
}