//
//  NoisyLayer.cpp
//  RebornOfBall
//
//  Created by LiuGloria on 15/6/5.
//
//

#include "NoisyLayer.h"

NoisyLayer::NoisyLayer(){
    brick_count = 0;
    boss_life_point = 10;
}

NoisyLayer::~NoisyLayer(){
}

bool NoisyLayer::init(){
    if(!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    // add the background
    auto sprite = Sprite::create("bg_noisy.jpg");
    //sprite->setScale(0.5f);
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(sprite, -1);
    sprite->setOpacity(127.5f);
    
    initGame();
    
    return true;
}

void NoisyLayer::initGame(){
    
    srand(time(NULL));
    visibleSize = Director::getInstance()->getVisibleSize();
    
    
    for (int i=0; i< COLLIDED_NUMBER; i++) {
        collidedBodies[i] = NULL;
    }
    
    for (int j = 0; j < NOISY_BRICK_COUNT; j++) {
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
    
    addBoss();
    
    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(NoisyLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(NoisyLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(NoisyLayer::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    scheduleUpdate();
}

void NoisyLayer::addBoss(){
    bossFace = Sprite::create("huge_mouse_monster.png");
    Size bossSize = bossFace->getContentSize();
    
    b2BodyDef bossBodyDef;
    bossBodyDef.type = b2_staticBody;
    
    bossBodyDef.position = b2Vec2( visibleSize.width /2 / WORLDSCALE, visibleSize.height /2 / WORLDSCALE);
    bossBody = world->CreateBody(&bossBodyDef);
    
    bossFace->setPosition(bossBodyDef.position.x * WORLDSCALE, bossBodyDef.position.y * WORLDSCALE);
    addChild(bossFace,-1);
    
    bossBody->SetUserData(bossFace);
    
    bossFace->setName("boss_noisy");
    
    b2PolygonShape bossShape;
    bossShape.SetAsBox(bossSize.width / 2 / WORLDSCALE, bossSize.height / 2 / WORLDSCALE);
    b2FixtureDef bossFixDef;
    bossFixDef.shape = &bossShape;
    bossFixDef.density = 1.0f;
    bossFixDef.friction = 0.3f;
    bossFixDef.restitution = 0.2f;
    bossBody->CreateFixture(&bossFixDef);
}

void NoisyLayer::update(float dur){
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
    
    for (int i = 0; i < NOISY_BRICK_COUNT; i++) {
        if (bricks[i]) {
            bricks[i]->dealCollision(dur, monsterBody);
        }
        
    }
    
    world->ClearForces();
    world->DrawDebugData();
}

void NoisyLayer::dealCollision(float dur){
    for (b2ContactEdge *c = monsterBody->GetContactList();c;c = c->next){
        b2Contact *contact = c->contact;
        b2Body *bodyA = contact->GetFixtureA()->GetBody();
        b2Body *bodyB = contact->GetFixtureB()->GetBody();
        
        
        b2Body *collidedBody = (bodyA == monsterBody)?bodyB:bodyA;
        Sprite* sp = (Sprite*)collidedBody->GetUserData();
        
        if ((collidedBody == paddleBody) && contact->IsTouching() == true)
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
        
        
        if (sp && (sp->getName() == "boss_noisy") && contact->IsTouching() == true) {
            
            brickForceVec.x =monsterBody->GetPosition().x - collidedBody->GetPosition().x;
            brickForceVec.y =monsterBody->GetPosition().y - collidedBody->GetPosition().y;
            brickForceVec.Normalize();
            monsterBody->ApplyLinearImpulse(brickForceVec, monsterBody->GetWorldCenter(), true);
            oldVec = b2Vec2(monsterBody->GetLinearVelocity().x, monsterBody->GetLinearVelocity().y);
            oldVec.Normalize();
            monsterBody->SetLinearVelocity(b2Vec2(oldVec.x * MONSTER_SPEED, oldVec.y * MONSTER_SPEED));
            
            
            boss_life_point--;
            if (boss_life_point < 0) {
                //the boss is dead
                this->scheduleOnce(schedule_selector(NoisyLayer::dealBossDeletion), 0.05f);
            }
        }
    }
    
}

void NoisyLayer::dealBossDeletion(float dt){
    CCLOG("delete the boss!");
    
    if (bossBody != NULL) {
        world->DestroyBody(bossBody);
        bossBody = NULL;
    }
    
    if (bossFace != NULL) {
        removeChild(bossFace);
        bossFace = NULL;
    }
}


void NoisyLayer::initBrickGroup(){
    this->schedule(schedule_selector(NoisyLayer::generatingBricks), 1.0F, 40, 0.0F);
}

void NoisyLayer::generatingBricks(float dt){
    if (bricks[brick_count] == NULL) {
        bricks[brick_count] = new BasicBrick(world,visibleSize,this);
        //bricks[brick_count]->initWithCentralNode(brick_count, "brick_bubble.png", "brick");
        bricks[brick_count]->initAsSpiral(visibleSize.width/2, visibleSize.height/2 /*+ NOISY_BOSS_HEIGHT /2 + 24*/, "brick_bubble.png", "brick");
        
        brick_count++;
    }
}


