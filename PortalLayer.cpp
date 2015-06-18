//
//  PortalLayer.cpp
//  helloworld
//
//  Created by LiuGloria on 15/4/23.
//
//

#include "PortalLayer.h"

USING_NS_CC;



PortalLayer::PortalLayer(){
    m_x_Portal = 0;
    m_y_Portal = 0;
    
    mReflectX_Portal = 0;
    mReflectY_Portal = 0;
    
    //calculate the number of bricks in the array
    brick_count = 0;
}

PortalLayer::~PortalLayer(){
}

bool PortalLayer::init(){
    if(!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    // add the background
    auto sprite = Sprite::create("bg_portal.jpg");
    //sprite->setScale(0.5f);
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(sprite,-1);
    //sprite->setOpacity(127.5f);
    

    initGame();
    
    return true;
}


void PortalLayer::initGame(){
    
    srand(time(NULL));
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    
    for (int i=0; i< COLLIDED_NUMBER; i++) {
        collidedBodies[i] = NULL;
    }
    
    for (int j = 0; j < TOTAL_BRICK_COUNT; j++) {
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
    
    listener->onTouchBegan = CC_CALLBACK_2(PortalLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(PortalLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(PortalLayer::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    scheduleUpdate();
    
}



void PortalLayer::addPaddle(){
    m_vec2_1 = new Vector2((float)(visibleSize.width/2), (float)(visibleSize.height/2));
    m_vec2_2 = new Vector2((float)(visibleSize.width/2), (float)(visibleSize.height/2));
    
    t_bInit = false;
    m_bMouseDown = false;
    m_nAngle = 0;
    
    //////////////////////////////////////////////new paddle body//////////////////////////////////////
    paddleNode = Node::create();
    paddleNode->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    paddle = Sprite::create("portal_out.png");
    paddleNode->addChild(paddle);
    paddle->setPosition(-CIRCLE_RADIUS,0 );
    paddle->setRotation(90);
    this->addChild(paddleNode,-1);
    
    b2BodyDef paddleBodyDef;
    paddleBodyDef.type = b2_staticBody;
    paddleBodyDef.position = b2Vec2((visibleSize.width / 2 - CIRCLE_RADIUS)/ WORLDSCALE, (visibleSize.height / 2) / WORLDSCALE);
    paddleBodyDef.angle = CC_DEGREES_TO_RADIANS(90);
    b2PolygonShape paddleShape;
    paddleShape.SetAsBox( 42.0f /2 / WORLDSCALE,207.0f /2 / WORLDSCALE);
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
    
    
    //the portal paddle
    paddle2 = Sprite::create("portal_in.png");
    paddleNode->addChild(paddle2);
    paddle2->setPosition(CIRCLE_RADIUS,0);
    paddle2->setRotation(90);
    
    b2BodyDef paddleBodyDef2;
    paddleBodyDef2.type = b2_staticBody;
    //(2a-m,2b-n)
    //paddleBodyDef2.position = b2Vec2( (2 * paddleNode->getPosition().x -(visibleSize.width / 2 - CIRCLE_RADIUS))/ WORLDSCALE,
    //                                  (2 * paddleNode->getPosition().y -(visibleSize.height / 2)) / WORLDSCALE);
    paddleBodyDef2.position = b2Vec2((visibleSize.width / 2 + CIRCLE_RADIUS)/ WORLDSCALE, (visibleSize.height / 2) / WORLDSCALE);
    paddleBodyDef2.angle = CC_DEGREES_TO_RADIANS(90);
    b2PolygonShape paddleShape2;
    paddleShape2.SetAsBox( 42.0f /2 / WORLDSCALE,207.0f /2 / WORLDSCALE);
    
    b2FixtureDef paddleFixDef2;
    paddleFixDef2.shape = &paddleShape2;
    paddleFixDef2.density = 10.0f;
    paddleFixDef2.friction = 0.3;
    paddleFixDef2.restitution = 0.2f;
    paddleBody2 = world->CreateBody(&paddleBodyDef2);
    paddleBody2->CreateFixture(&paddleFixDef2);
    Vec2 position2 = paddleNode->convertToWorldSpace(paddle2->getPosition());
    paddleBody2->SetTransform(b2Vec2(position2.x,position2.y),CC_DEGREES_TO_RADIANS(paddleNode->getRotation()) );
    
    //printf("position of paddlebody2 is %f,%f\n",paddleBody2->GetPosition().x,paddleBody2->GetPosition().y);
    //printf("opposite paddle added!\n");
    
}

void PortalLayer::initMonsterBody(){
    if (monster)
    {
        removeChild(monster);
    }
    monster  = NULL;
    
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


void PortalLayer::calInterPoint_Portal(){
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
        m_x_Portal = x0;
        if (x2 - x1 != 0) {
            m_y_Portal = (y2 - y1) * (m_x_Portal - x1) / (x2 - x1) + y1;
        }
        
    }
    if (v.x != 0) {
        k = v.y / v.x;
        if (y2 - y1 - k * (x2 - x1) != 0) {
            m_x_Portal = ( (x2 - x1) * (y0 - y1 - k * x0) + x1 * y2 - x1 * y1) / (y2 - y1 - k * (x2 - x1));
            m_y_Portal = k * (m_x_Portal - x0) + y0;
        }
    }
    
}

void PortalLayer::update(float dur){
    int velIteration = 8;
    int postIteration = 3;
    world->Step(dur, velIteration, postIteration);
    
    //synchronizing its skin
    monster->setPosition(monsterBody->GetPosition().x * WORLDSCALE, monsterBody->GetPosition().y * WORLDSCALE);
    monster->setRotation(CC_RADIANS_TO_DEGREES(monsterBody->GetAngle()));
    
    
    //visualization
    calInterPoint();
    calReflectedRay();
    calInterPoint_Portal();
    
    drawNode->clear();
    Vec2 dirVec = Vec2(monster->getPosition().x - paddleNode->getPosition().x, monster->getPosition().y - paddleNode->getPosition().y);
    float multiNum = dirVec.x * monsterBody->GetLinearVelocity().x + dirVec.y * monsterBody->GetLinearVelocity().y;
    if (monster->getPosition().distance(paddleNode->getPosition()) > MONSTER_RADIUS
        && monster->getPosition().distance(paddleNode->getPosition()) < CIRCLE_RADIUS)
    {
        //if (multiNum > 0) {
            //draw the white line
            drawNode->drawSegment(Vec2(monster->getPosition().x, monster->getPosition().y), Vec2(m_x, m_y), 3.0f, Color4F(1, 1, 1, 1));
            drawNode->drawSegment(Vec2(m_x, m_y), Vec2(mReflectX, mReflectY), 3.0f, Color4F(1, 1, 1, 1));
        //}else{
            drawNode->drawSegment(Vec2(monster->getPosition().x, monster->getPosition().y), Vec2(m_x_Portal, m_y_Portal), 3.0f, Color4F(0.8, 0.8, 0.8, 1));
            drawNode->drawSegment(Vec2(2 * paddleNode->getPosition().x - m_x_Portal,
                                    2 * paddleNode->getPosition().y - m_y_Portal),
                               Vec2(2 * paddleNode->getPosition().x - monster->getPosition().x,
                                    2 * paddleNode->getPosition().y - monster->getPosition().y), 3.0f, Color4F(0.6, 0.6, 0.6, 1));
        //}
    }
    
    this->dealCollision(dur);
    
    for (int i = 0; i < TOTAL_BRICK_COUNT; i++) {
        if (bricks[i] != NULL) {
            bricks[i]->dealCollision(dur, monsterBody);
        }
        
    }
    
    
    world->ClearForces();
    world->DrawDebugData();
}



void PortalLayer::dealCollision(float dur){
    for (b2ContactEdge *c = monsterBody->GetContactList();c;c = c->next){
        b2Contact *contact = c->contact;
        b2Body *bodyA = contact->GetFixtureA()->GetBody();
        b2Body *bodyB = contact->GetFixtureB()->GetBody();
        
        
        b2Body *collidedBody = (bodyA == monsterBody)?bodyB:bodyA;
        
        if (collidedBody == paddleBody && contact->IsTouching() == true)
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
        
        //collide with the portal paddle
        if (collidedBody == paddleBody2 && contact->IsTouching() == true)
        {
            Vec2 currentPos = Vec2(monster->getPosition().x, monster->getPosition().y);
            Vec2 nextFramePos = Vec2(2 * paddleNode->getPosition().x - currentPos.x,
                                     2 * paddleNode->getPosition().y - currentPos.y);
            float currentAngle = monsterBody->GetAngle();
            
            
            //reset position of the monster,while the velocity remains unchanged
            monsterBody->SetTransform(b2Vec2(nextFramePos.x / WORLDSCALE, nextFramePos.y / WORLDSCALE), currentAngle);
            oldVec = b2Vec2(monsterBody->GetLinearVelocity().x, monsterBody->GetLinearVelocity().y);
            oldVec.Normalize();
            monsterBody->SetLinearVelocity(b2Vec2(oldVec.x * MONSTER_SPEED, oldVec.y * MONSTER_SPEED));

            return;
        }

        
//        Sprite* sp = (Sprite*)collidedBody->GetUserData();
//        if (sp && (sp->getName() == "brick" ||  sp->getName() == "wall") && contact->IsTouching() == true) {
//            
//            
//            brickForceVec.x =monsterBody->GetPosition().x - collidedBody->GetPosition().x;
//            brickForceVec.y =monsterBody->GetPosition().y - collidedBody->GetPosition().y;
//            brickForceVec.Normalize();
//            monsterBody->ApplyLinearImpulse(brickForceVec, monsterBody->GetWorldCenter(), true);
//            oldVec = b2Vec2(monsterBody->GetLinearVelocity().x, monsterBody->GetLinearVelocity().y);
//            oldVec.Normalize();
//            monsterBody->SetLinearVelocity(b2Vec2(oldVec.x * MONSTER_SPEED, oldVec.y * MONSTER_SPEED));
//            
//            /*if (sp->getName() == "brick") {
//                for (int i = 0; collidedBodies[i] == NULL; i++) {
//                    collidedBodies[i] = collidedBody;
//                    break;
//                }
//                this->scheduleOnce(schedule_selector(PortalLayer::dealDeletion),0.05f);
//            }*/
//            
//        }
    }
    

}

bool PortalLayer::onTouchBegan( Touch* touch, Event *event )
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

void PortalLayer::onTouchMoved( Touch* touch, Event *event)
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

void PortalLayer::initBrickGroup()
{
	addBrickLine("redBrick.jpg","wall",0,0,3,1);

	addBrickLine("normalBrick.jpg","brick",1 ,0.5f,2,1);

	addBrickLine("blueBrick.jpg","brick",2 ,-0.5f,1,0);
	addBrickLine("normalBrick.jpg","brick",2 ,1.0f,1,0);

	addBrickLine("blueBrick.jpg","brick",3 ,0.5f,1,0);

	addBrickLine("normalBrick.jpg","brick",4,0,1,1);

	addBrickLine("blueBrick.jpg","brick",5 ,-0.5f,1,0);

	addBrickLine("blueBrick.jpg","brick",-1 ,-0.5f,2,2);

	addBrickLine("normalBrick.jpg","brick",-2 ,1.0f,1,0);

	addBrickLine("normalBrick.jpg","brick",-3 ,0.5f,1,0);

	addBrickLine("blueBrick.jpg","brick",-4 ,-0.5f,1,0);

	

	
	////ÖÐ¼äÒ»ÅÅÇ½
	//auto redBrick = Sprite::create("redBrick.jpg");
	//Size brickSize = redBrick->getContentSize();
	//for (int i = 0;i<2;i++)
	//{
	//	float x1= visibleSize.width/2 +  brickSize.width/2 +brickSize.width * i;
	//	float x2 = visibleSize.width/2 -  brickSize.width/2 - brickSize.width * i;
	//	float y = visibleSize.height/2;
	//	addBrick(x1,y,"redBrick.jpg","wall");
	//	addBrick(x2,y,"redBrick.jpg","wall");
	//}
	//


}

void PortalLayer::addBrickLine( std::string filename,std::string name,int lineSequence,float marginScale,float num,float gapScale )
{
	auto Brick = Sprite::create(filename);
	Size brickSize = Brick->getContentSize();
	for (int i = 0;i<num ;i++)
	{
		float x1 = visibleSize.width /  2 + (marginScale + 0.5) * brickSize.width +brickSize.width * gapScale * i;
		float x2 = visibleSize.width / 2 - (marginScale + 0.5) * brickSize.width - brickSize.width * gapScale * i;
		float y = visibleSize.height / 2 + brickSize.height * lineSequence;
        
		//addBrick(x1,y,filename,name);
		
        if (bricks[brick_count] == NULL) {
            bricks[brick_count] = new BasicBrick(world,visibleSize,this);
            bricks[brick_count]->init(x1,y,filename,name);
            
            brick_count++;
        }
        
        if (x1 != x2)
		{
			//addBrick(x2,y,filename,name);
            
            if (bricks[brick_count] == NULL) {
                bricks[brick_count] = new BasicBrick(world,visibleSize,this);
                bricks[brick_count]->init(x2,y,filename,name);
                
                brick_count++;
            }
		}
	}
}
