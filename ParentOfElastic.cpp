//
//  BackgroundLayer.h
//  helloworld
//
//  Created by LiuGloria on 15/4/16.
//
//

#include "ParentOfElastic.h"

USING_NS_CC;


ParentOfElastic::ParentOfElastic(){
}

ParentOfElastic::~ParentOfElastic(){
}

bool ParentOfElastic::init(){
    if(!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    // add the background
    auto sprite = Sprite::create("bg.jpg");
    //sprite->setScale(0.5f);
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    initGame();

    return true;
}

void ParentOfElastic::initGame(){
    
    srand(time(NULL));
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    
    for (int i=0; i< COLLIDED_NUMBER; i++) {
        collidedBodies[i] = NULL;
    }
    
    
    float angle = MATH_DEG_TO_RAD(180);
    
    
    initPhysics();
    
    //initDebugDraw();
    
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
    
    listener->onTouchBegan = CC_CALLBACK_2(ParentOfElastic::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ParentOfElastic::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ParentOfElastic::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    scheduleUpdate();
    
}

void ParentOfElastic::initPhysics(){
    world = new b2World(b2Vec2(0,0));
    world->SetContactListener(this);
    world->SetContinuousPhysics(true);
    
}

void ParentOfElastic::initDebugDraw(){
    debugDraw = new GLESDebugDraw(WORLDSCALE);
    world->SetDebugDraw(debugDraw);
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    
    debugDraw->SetFlags(flags);
    world->SetDebugDraw(debugDraw);
}

void ParentOfElastic::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags){
    //Layer::draw(renderer, transform, flags);
    //world->DrawDebugData();
    
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    world->DrawDebugData();
    CHECK_GL_ERROR_DEBUG();
}

void ParentOfElastic::calInterPoint(){
    Vec2 v1 = paddleNode->convertToWorldSpace( Vec2(paddle->getBoundingBox().getMaxX(),paddle->getBoundingBox().getMinY()) );
    Vec2 v2 = paddleNode->convertToWorldSpace( Vec2(paddle->getBoundingBox().getMaxX(),paddle->getBoundingBox().getMaxY()) );
    
    //b2Vec2 v1 = paddleBody->GetWorldVector(b2Vec2(0, 1));
    //b2Vec2 v2 = paddleBody->GetWorldVector(b2Vec2(0, -1));
    
    
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
        m_x = x0;
        if (x2 - x1 != 0) {
            m_y = (y2 - y1) * (m_x - x1) / (x2 - x1) + y1;
        }
        
    }
    if (v.x != 0) {
        k = v.y / v.x;
        if (y2 - y1 - k * (x2 - x1) != 0) {
            m_x = ( (x2 - x1) * (y0 - y1 - k * x0) + x1 * y2 - x1 * y1) / (y2 - y1 - k * (x2 - x1));
            m_y = k * (m_x - x0) + y0;
        }
    }
    
}

void ParentOfElastic::calReflectedRay(){
    Vec2 vec1 = paddleNode->convertToWorldSpace( Vec2(paddle->getBoundingBox().getMaxX(),paddle->getBoundingBox().getMaxY()) );
    Vec2 vec2 = paddleNode->convertToWorldSpace( Vec2(paddle->getBoundingBox().getMinX(),paddle->getBoundingBox().getMaxY()) );

    vec1.subtract(vec2);
    Vec2 normalVec = vec1;
    normalVec.normalize();
    
    
    float x_1 = m_x + 357 * normalVec.x;
    float y_1 = m_y + 357 * normalVec.y;
    float x_2 = m_x;
    float y_2 = m_y;
    float x_0 = monster->getPosition().x;
    float y_0 = monster->getPosition().y;
    
    
    float A_ = y_2 - y_1;
    float B_ = x_1 - x_2;
    float C_ = x_2 * y_1 - x_1 * y_2;
    float k_ = -2 * (A_ * x_0 + B_ * y_0 + C_) / (A_ * A_ + B_ * B_);
    
    if (x_2 - x_1 != 0 && y_2 - y_1 != 0) {
        mReflectX = x_0 + k_ * A_;
        mReflectY = y_0 + k_ * B_;
    }
}

void ParentOfElastic::update(float dur){
    int velIteration = 8;
    int postIteration = 3;
    world->Step(dur, velIteration, postIteration);
    
	onUpdate();

    //synchronizing its skin
    monster->setPosition(monsterBody->GetPosition().x * WORLDSCALE, monsterBody->GetPosition().y * WORLDSCALE);
    monster->setRotation(CC_RADIANS_TO_DEGREES(monsterBody->GetAngle()));
    

   drawNode->clear();
    Vec2 dirVec = Vec2(monster->getPosition().x - paddleNode->getPosition().x, monster->getPosition().y - paddleNode->getPosition().y);
    float multiNum = dirVec.x * monsterBody->GetLinearVelocity().x + dirVec.y * monsterBody->GetLinearVelocity().y;
    if (/*multiNum > 0 &&*/ monster->getPosition().distance(paddleNode->getPosition()) > MONSTER_RADIUS
        && monster->getPosition().distance(paddleNode->getPosition()) < CIRCLE_RADIUS)
    {
		//visualization
		calInterPoint();
		calReflectedRay();

        drawNode->drawSegment(Vec2(monster->getPosition().x, monster->getPosition().y), Vec2(m_x, m_y), 3.0f, Color4F(1, 0, 0, 1));
        drawNode->drawSegment(Vec2(m_x, m_y), Vec2(mReflectX, mReflectY), 3.0f, Color4F(1, 0, 0, 1));
    }
    
    /*String* monsterMass = String::createWithFormat("%f",monsterBody->GetMass());
     LabelTTF *label;
     label->setString(&monsterMass);
     label->setString(monsterBody->GetMass())
     LabelTTF *label = LabelTTF::labelWithString("hello world", CCSizeMake(100, 25), (CCTextAlignment)intAlign, strFontName, fontSize);
     label->setAnchorPoint(ccp(0, 0));
     label->setPosition = ccp(100,100);*/
    
    this->dealCollision(dur);
    
    world->ClearForces();
    world->DrawDebugData();
}

void ParentOfElastic::addPaddle(){
    //float x,y,width,height;
    //x =  0;
    //y =  maxRadius - miniRadius * sin(angle/2);
    //width =  maxRadius - miniRadius ;
    //height = 2 * miniRadius * sin(angle/2);
    //Rect rect = Rect(x,y,width,height);
    //auto clip = Sprite::create("for_cut.png",rect);
    ////Size paddleSize = paddle->getContentSize();
    //clip->setAnchorPoint(Vec2(0,0));
    ////Size visibleSize = Director::getInstance()->getVisibleSize();
    //paddle = Sprite::create("empty_bg.png");
    //paddle->addChild(clip);
    //clip->setPosition(x,y);
    //clip->setName("clip");
    //paddle->setPosition(visibleSize.width/2, visibleSize.height/2);
    ////paddle->setScale(m_scale/2);
    //
    //addChild(paddle);
    
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
}

void ParentOfElastic::addBrick(int i, int j){
    Sprite *brick = Sprite::create("brick.png");
    Size brickSize = brick->getContentSize();
    
    b2BodyDef brickBodyDef;
    brickBodyDef.type = b2_staticBody;
    
    float disX = visibleSize.width /2 - (BRICK_COLUMN * BRICK_LENGTH + (BRICK_COLUMN - 1) * BRICK_GAP) /2 ;
    float disY = visibleSize.height /2 - (BRICK_COLUMN * BRICK_LENGTH + (BRICK_COLUMN - 1) * BRICK_GAP) /2 ;
    
    brickBodyDef.position = b2Vec2( (disX + i * (BRICK_LENGTH + BRICK_GAP)) / WORLDSCALE, (disY + j * (BRICK_LENGTH + BRICK_GAP)) / WORLDSCALE);
    b2Body *brickBody = world->CreateBody(&brickBodyDef);
    
    brick->setPosition(brickBodyDef.position.x * WORLDSCALE, brickBodyDef.position.y * WORLDSCALE);
    addChild(brick);
    
    brickBody->SetUserData(brick);
    
    brick->setName("brick");
    brick->setTag(5);
    b2PolygonShape brickShape;
    brickShape.SetAsBox(brickSize.width / 2 / WORLDSCALE, brickSize.height / 2 / WORLDSCALE);
    b2FixtureDef brickFixDef;
    brickFixDef.shape = &brickShape;
    brickFixDef.density = 1.0f;
    brickFixDef.friction = 0.3f;
    brickFixDef.restitution = 0.2f;
    brickBody->CreateFixture(&brickFixDef);
}

void ParentOfElastic::addBrick(float posx,float posy,std::string filename,std::string name )
{
	Sprite *brick = Sprite::create(filename);
	Size brickSize = brick->getContentSize();

	b2BodyDef brickBodyDef;
	brickBodyDef.type = b2_staticBody;

	brickBodyDef.position = b2Vec2(posx / WORLDSCALE,posy / WORLDSCALE);
	b2Body *brickBody = world->CreateBody(&brickBodyDef);

	brick->setPosition(brickBodyDef.position.x * WORLDSCALE, brickBodyDef.position.y * WORLDSCALE);
	addChild(brick);

	brickBody->SetUserData(brick);

	brick->setName(name);

	b2PolygonShape brickShape;
	brickShape.SetAsBox(brickSize.width / 2 / WORLDSCALE, brickSize.height / 2 / WORLDSCALE);
	b2FixtureDef brickFixDef;
	brickFixDef.shape = &brickShape;
	brickFixDef.density = 1.0f;
	brickFixDef.friction = 0.3f;
	brickFixDef.restitution = 0.2f;
	brickBody->CreateFixture(&brickFixDef);
}


void ParentOfElastic::addMonsterBody(){
    b2BodyDef monsterBodyDef;
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
    monsterBody->CreateFixture(&monsterFixDef);
    
	//monster = Sprite::create("ball.png");
	//Size monsterSize = monster->getContentSize();

	//monster->setPosition(monsterBody->GetPosition().x * WORLDSCALE, monsterBody->GetPosition().y * WORLDSCALE);
	//addChild(monster);
	//monsterBody->SetUserData(monster);

}

void ParentOfElastic::initMonsterBody(){
	if (monster)
	{
		removeChild(monster);
	}
	monster  = NULL;
	
	//monster = rand()%2 == 0 ? Sprite::create("whiteBall.png") : Sprite::create("blackBall.png");
	monster = Sprite::create("ball_demo.png");
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

void ParentOfElastic::dealDeletion(float dt){
    //CCLOG("call dealDeletion once");
    b2Body *bodyToDelete;
    Sprite* sp;
    
    for (int i=0; i<COLLIDED_NUMBER; i++) {
        if (collidedBodies[i]) {
            bodyToDelete = collidedBodies[i];
            if (bodyToDelete->GetUserData()) {
                sp = (Sprite*)bodyToDelete->GetUserData();
                
                world->DestroyBody(bodyToDelete);
                removeChild(sp);
				sp = NULL;
            }
        }
        
    }
    for (int i=0; i<COLLIDED_NUMBER; i++) {
        if (collidedBodies[i]) {
            collidedBodies[i] = NULL;
        }
        
    }
}

void ParentOfElastic::dealCollision(float dur){

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
            monsterBody->SetLinearVelocity(b2Vec2(oldVec.x * 10, oldVec.y * 10));
            return;
        }
        
        Sprite* sp = (Sprite*)collidedBody->GetUserData();
        if (sp && (sp->getName() == "brick" || sp->getName() == "wall" || sp->getName() == "boss") && contact->IsTouching() == true) {
            
            brickForceVec.x =monsterBody->GetPosition().x - collidedBody->GetPosition().x;
            brickForceVec.y =monsterBody->GetPosition().y - collidedBody->GetPosition().y;
            brickForceVec.Normalize();
            monsterBody->ApplyLinearImpulse(brickForceVec, monsterBody->GetWorldCenter(), true);
            oldVec = b2Vec2(monsterBody->GetLinearVelocity().x, monsterBody->GetLinearVelocity().y);
            oldVec.Normalize();
            monsterBody->SetLinearVelocity(b2Vec2(oldVec.x * 10, oldVec.y * 10));

			
			/*sp->setTag(sp->getTag() - 1);

			if (sp->getTag() == 0)
			{
			for (int i = 0; collidedBodies[i] == NULL; i++) {
			collidedBodies[i] = collidedBody;
			break;
			}
			this->scheduleOnce(schedule_selector(ParentOfElastic::dealDeletion),0.05f);
			}*/
			
        }

		if (sp && sp->getName() == "brick")
		{
			removeJudgment(collidedBody);
		}
		
		onDealCollision(collidedBody);
		
    }
        
}

void ParentOfElastic::BeginContact(b2Contact* contact){

}

void ParentOfElastic::EndContact(b2Contact* contact){
    
}

bool ParentOfElastic::onTouchBegan( Touch* touch, Event *event )
{
    monsterBody->SetTransform(b2Vec2(572 / WORLDSCALE, 280 / WORLDSCALE), 0);
    initMonsterBody();
    
    auto location = touch->getLocation();
    
    Vec2 relatedLocation = paddleNode->convertToNodeSpace(touch->getLocation());
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

void ParentOfElastic::onTouchMoved( Touch* touch, Event *event)
{
    auto location = touch->getLocation();
    
    if(!m_bMouseDown){
        return;
    }
    
    m_vec2_2->updatefunc(location.x ,location.y);
    m_nAngle = Vector2::Angle(*m_vec2_1 , *m_vec2_2);//degree
    m_vec2_1->updatefunc(location.x ,location.y);
    
    
    float p_rotation = paddleNode->getRotation();

	if (p_rotation + m_nAngle > 360)
	{
		p_rotation -= 360;
	}

    paddleNode->setRotation(p_rotation + m_nAngle);
    
    Vec2 position = paddleNode->convertToWorldSpace(paddle->getPosition());
    b2Vec2 p_position = b2Vec2(position.x / WORLDSCALE, position.y / WORLDSCALE);
    float p_angle = -( CC_DEGREES_TO_RADIANS(paddleNode->getRotation()) );
    paddleBody->SetTransform(p_position,p_angle);
    
    m_nAngle = 0.0;
    
}

void ParentOfElastic::onTouchEnded( Touch* touch, Event *event )
{
    if(m_bMouseDown){
        m_bMouseDown=false;
        m_vec2_1->updatefunc();
        m_vec2_2->updatefunc();
        m_nAngle=0;
    }
}

void ParentOfElastic::initBrickGroup()
{
	for (int i = 0; i < BRICK_COLUMN; i++) {
		for (int j = 0; j < BRICK_COLUMN; j++) {
			addBrick(i, j);
		}
	}
}

void ParentOfElastic::onUpdate()
{

}

void ParentOfElastic::onDealCollision(b2Body* collidedBody)
{

}

void ParentOfElastic::removeJudgment( b2Body* collidedBody)
{
	Sprite *sp = (Sprite *)collidedBody->GetUserData();

	sp->setTag(sp->getTag() - 1);

	if (sp->getTag() <= 0)
	{
		for (int i = 0; collidedBodies[i] == NULL; i++) {
			collidedBodies[i] = collidedBody;
			break;
		}
		this->scheduleOnce(schedule_selector(ParentOfElastic::dealDeletion),0.05f);
	}
}














