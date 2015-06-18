//
//  GravityLayer.cpp
//  RebornOfBall
//
//  Created by LiuGloria on 15/4/30.
//
//

#include "GravityLayer.h"

USING_NS_CC;


GravityLayer::GravityLayer(){
}

GravityLayer::~GravityLayer(){
}



/*void GravityLayer::addPaddle(){
    
    m_vec2_1 = new Vector2((float)(visibleSize.width/2), (float)(visibleSize.height/2));
    m_vec2_2 = new Vector2((float)(visibleSize.width/2), (float)(visibleSize.height/2));
    
    t_bInit = false;
    m_bMouseDown = false;
    m_nAngle = 0;
    
    //////////////////////////////////////////////new paddle body//////////////////////////////////////
    paddleNode = Node::create();
    paddleNode->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    paddle = Sprite::create("paddleBody.png");
    paddleNode->addChild(paddle);
    paddle->setPosition(- CIRCLE_RADIUS,0 );
    paddle->setRotation(90);
    this->addChild(paddleNode,-1);
    
    b2BodyDef paddleBodyDef;
    paddleBodyDef.type = b2_staticBody;
    paddleBodyDef.position = b2Vec2((visibleSize.width / 2 - CIRCLE_RADIUS)/ WORLDSCALE, (visibleSize.height / 2) / WORLDSCALE);
    paddleBodyDef.angle = CC_DEGREES_TO_RADIANS(90);
    b2PolygonShape paddleShape;
    paddleShape.SetAsBox( PADDLE_THICKNESS/2 / WORLDSCALE,160.f /2 / WORLDSCALE);
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
    
    
    
    
    leftWing = Sprite::create("paddleLeftWing.png");
    paddleNode->addChild(leftWing);
    leftWing->setPosition(- CIRCLE_RADIUS,70 );
    leftWing->setRotation(90);
    b2BodyDef leftWingBodyDef;
    
    //1.set type for wallBrick:kinematic
    leftWingBodyDef.type = b2_kinematicBody;
    //2.set up shape:polygon
    b2PolygonShape leftWingShape;
    b2Vec2 vertices[4] = {
        b2Vec2(-40/ WORLDSCALE, -10/ WORLDSCALE),
        b2Vec2( 40/ WORLDSCALE,  70/ WORLDSCALE),
        b2Vec2(-40/ WORLDSCALE, -70/ WORLDSCALE),
        b2Vec2( 40/ WORLDSCALE, -70/ WORLDSCALE)
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
    leftWingBody->SetUserData(leftWing);
    leftWing->setName("leftWing");
    Vec2 leftWingPosition = paddleNode->convertToWorldSpace(leftWing->getPosition());
    leftWingBody->SetTransform(b2Vec2(leftWingPosition.x,leftWingPosition.y),CC_DEGREES_TO_RADIANS(paddleNode->getRotation()) );

    
    rightWing = Sprite::create("paddleRightWing.png");
    paddleNode->addChild(rightWing);
    rightWing->setPosition(- CIRCLE_RADIUS,-70 );
    rightWing->setRotation(90);
    b2BodyDef rightWingBodyDef;
    
    //1.set type for wallBrick:kinematic
    rightWingBodyDef.type = b2_kinematicBody;
    //2.set up shape:polygon
    b2PolygonShape rightWingShape;
    b2Vec2 r_vertices[4] = {
        b2Vec2(-40/ WORLDSCALE,  70/ WORLDSCALE),
        b2Vec2( 40/ WORLDSCALE,  70/ WORLDSCALE),
        b2Vec2( 40/ WORLDSCALE, -70/ WORLDSCALE),
        b2Vec2(-40/ WORLDSCALE,  10/ WORLDSCALE)
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
    rightWingBody->SetUserData(rightWing);
    rightWing->setName("rightWing");
    Vec2 rightWingPosition = paddleNode->convertToWorldSpace(rightWing->getPosition());
    rightWingBody->SetTransform(b2Vec2(rightWingPosition.x,rightWingPosition.y),CC_DEGREES_TO_RADIANS(paddleNode->getRotation()) );

}*/


void GravityLayer::update(float dur){
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
    
    
    //apply the gravity which is always pointing to the centre, in a given area
    float forceVecX =paddleNode->getPosition().x;
    float forceVecY =paddleNode->getPosition().y;
    Vec2 forceDirVec = Vec2(monster->getPosition().x - forceVecX,
                            monster->getPosition().y - forceVecY);
    if (monster->getPosition().distance(Vec2(forceVecX, forceVecY)) < FORCE_AREA){
        //monsterBody->ApplyForceToCenter(b2Vec2(-dirVec.x * 10000 / WORLDSCALE, -dirVec.y * 10000 / WORLDSCALE), true);
        //monsterBody->ApplyForce(b2Vec2(-dirVec.x * 10000 / WORLDSCALE, -dirVec.y * 10000 / WORLDSCALE),
        //                        b2Vec2(paddleNode->getPosition().x / WORLDSCALE, paddleNode->getPosition().y / WORLDSCALE), true);
        
        monsterBody->ApplyLinearImpulse(b2Vec2(-forceDirVec.x / 7/ WORLDSCALE, -forceDirVec.y / 7/ WORLDSCALE), monsterBody->GetWorldCenter(), true);
    
        oldVec = b2Vec2(monsterBody->GetLinearVelocity().x, monsterBody->GetLinearVelocity().y);
        oldVec.Normalize();
        monsterBody->SetLinearVelocity(b2Vec2(oldVec.x * MONSTER_SPEED, oldVec.y * MONSTER_SPEED));
    }
    drawNode->drawCircle(Vec2(forceVecX, forceVecY), FORCE_AREA, 0.0f, 360, true, 1, 1, Color4F(0, 1, 0, 1));
    
    
    

    
    
    this->dealCollision(dur);
    
    for (int i = 0; i < BRICK_COUNT; i++) {
        bricks[i]->dealCollision(dur, monsterBody);
    }
    
    world->ClearForces();
    world->DrawDebugData();
}


/*void GravityLayer::onTouchMoved( Touch* touch, Event *event)
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
    
    Vec2 leftWingPos = paddleNode->convertToWorldSpace(leftWing->getPosition());
    b2Vec2 l_position = b2Vec2(leftWingPos.x / WORLDSCALE, leftWingPos.y / WORLDSCALE);
    leftWingBody->SetTransform(l_position,p_angle);
    
    
    Vec2 rightWingPos = paddleNode->convertToWorldSpace(rightWing->getPosition());
    b2Vec2 r_position = b2Vec2(rightWingPos.x / WORLDSCALE, rightWingPos.y / WORLDSCALE);
    rightWingBody->SetTransform(r_position,p_angle);
    
    
    m_nAngle = 0.0;
    
}*/