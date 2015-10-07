#include "PiecesLayer.h"


PiecesLayer::PiecesLayer(void)
{
}


PiecesLayer::~PiecesLayer(void)
{
}

void PiecesLayer::addPaddle()
{
	m_vec2_1 = new Vector2((float)(visibleSize.width/2), (float)(visibleSize.height/2));
	m_vec2_2 = new Vector2((float)(visibleSize.width/2), (float)(visibleSize.height/2));

	t_bInit = false;
	m_bMouseDown = false;
	m_nAngle = 0;

	paddleNode = Node::create();
	paddleNode->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	
	for (int i = 0;i < 12;i++)
	{
		Paddles[i] = new Paddle(i,world,paddleNode);
	}
	//for (int i = 0;i < 12;i++)
	//{
	//	auto paddle = Sprite::create("30DegreePaddle.png");
	//	paddle->setName("paddle");
	//	paddleNode->addChild(paddle);
	//	paddle->setRotation(150 - 30 * i);
	//	float posX = 0 - (CIRCLE_RADIUS - PIECES_PADDLE_THICKNESS/4) * cos(CC_DEGREES_TO_RADIANS(15))*sin(CC_DEGREES_TO_RADIANS(paddle->getRotation()));
	//	float posY = 0 - (CIRCLE_RADIUS - PIECES_PADDLE_THICKNESS/4) * cos(CC_DEGREES_TO_RADIANS(15))*cos(CC_DEGREES_TO_RADIANS(paddle->getRotation()));
	//	paddle->setPosition(posX,posY);
	//	paddle->setTag(i);

	//	b2BodyDef paddleBodyDef;
	//	paddleBodyDef.type = b2_staticBody;
	//	//paddleBodyDef.position = b2Vec2((visibleSize.width / 2 - CIRCLE_RADIUS)/ WORLDSCALE, (visibleSize.height / 2) / WORLDSCALE);

	//	b2PolygonShape paddleShape;
	//	paddleShape.SetAsBox( PIECES_PADDLE_LENGTH /2 / WORLDSCALE,PIECES_PADDLE_THICKNESS/2 / WORLDSCALE);

	//	b2FixtureDef paddleFixDef;
	//	paddleFixDef.shape = &paddleShape;
	//	paddleFixDef.density = 10.0f;
	//	paddleFixDef.friction = 0.3;
	//	paddleFixDef.restitution = 0.2f;

	//	b2Body *paddleBody = world->CreateBody(&paddleBodyDef);
	//	paddleBody->CreateFixture(&paddleFixDef);
	//	Vec2 position = paddleNode->convertToWorldSpace(paddle->getPosition());
	//	paddleBody->SetTransform(b2Vec2(position.x,position.y),CC_DEGREES_TO_RADIANS(paddleNode->getRotation() + 150 - 30 * i) );
	//	
	//	paddleBody->SetUserData(paddle);

	//	paddleBodies[i] = paddleBody;
	//	paddles[i] = paddle;

	//	Paddles[i] = new Paddle(paddle);
	//}

	
	this->addChild(paddleNode);

}

void PiecesLayer::calInterPoint()
{
	
	for (int i = 0; i < 12; i ++)
	{
		Sprite* m_paddle = Paddles[i]->m_paddle;
		if (m_paddle->getName() == "paddle" )
		{
			Vec2 v1,v2;
			float angle =  CC_DEGREES_TO_RADIANS(paddleNode->getRotation() + 150 - 30 * i);
			Vec2 directVec = Vec2(cos(angle),-sin(angle));//?
			v1 = paddleNode->convertToWorldSpace(m_paddle->getPosition()) + PIECES_PADDLE_LENGTH / 2 * directVec;
			v2 = paddleNode->convertToWorldSpace(m_paddle->getPosition()) - PIECES_PADDLE_LENGTH / 2 * directVec;
			//drawNode->drawSegment(v1, v2, 3.0f, Color4F(1, 1, 1, 1));
			//calculate inter point and judge if inter point is on the line
			if (calInterPoint(monster->getPosition(),monsterBody->GetLinearVelocity(),v1,v2))
			{
				calReflectedRay(monster->getPosition(),paddleNode->convertToWorldSpace(m_paddle->getPosition()),paddleNode->getPosition());
				return;
			} //else{ m_x = 0;m_y = 0;}
		}

	}
		

}

bool PiecesLayer::calInterPoint( Vec2 linePoint,b2Vec2 directVec,Vec2 segmentVertax1,Vec2 segmentVertax2 )
{
	Vec2 v1 = segmentVertax1;
	Vec2 v2 = segmentVertax2;

	float x1 = v1.x;float y1 = v1.y;
	float x2 = v2.x;float y2 = v2.y;			

	b2Vec2 v = directVec;
	float k = 0;
	//monster position
	float x0 = linePoint.x;float y0 = linePoint.y;

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

	//if inter point is on the line
	if ( (m_x - v1.x)*(m_x - v2.x) < 0 )
	{
		return true;
	}

	return false;
}

void PiecesLayer::calReflectedRay()
{

}

void PiecesLayer::calReflectedRay( Vec2 tobeReflectPoint,Vec2 symmetryAxisVertax1,Vec2 symmetryAxisVertax2)
{
	Vec2 vec1 = symmetryAxisVertax1;
	Vec2 vec2 = symmetryAxisVertax2;

	vec1.subtract(vec2);

	float x_1,y_1,x_2,y_2;

	Vec2 normalVec = vec1;
	normalVec.normalize();
	x_1 = m_x - CIRCLE_RADIUS * normalVec.x;
	y_1 = m_y - CIRCLE_RADIUS * normalVec.y;
	x_2 = m_x;
	y_2 = m_y;

	//drawNode->drawSegment(Vec2(m_x, m_y), Vec2(x_1, y_1), 3.0f, Color4F(1, 1, 1, 1));
	float x_0 = tobeReflectPoint.x;
	float y_0 = tobeReflectPoint.y;

	float A_ = y_2 - y_1;
	float B_ = x_1 - x_2;
	float C_ = x_2 * y_1 - x_1 * y_2;
	float k_ = -2 * (A_ * x_0 + B_ * y_0 + C_) / (A_ * A_ + B_ * B_);

	if (x_2 - x_1 != 0 && y_2 - y_1 != 0) {
		mReflectX = x_0 + k_ * A_;
		mReflectY = y_0 + k_ * B_;
	}
}

void PiecesLayer::dealCollision( float dur ){
	for (b2ContactEdge *c = monsterBody->GetContactList();c;c = c->next){
		b2Contact *contact = c->contact;
		b2Body *bodyA = contact->GetFixtureA()->GetBody();
		b2Body *bodyB = contact->GetFixtureB()->GetBody();


		b2Body *collidedBody = (bodyA == monsterBody)?bodyB:bodyA;
		Sprite* sp = (Sprite*)collidedBody->GetUserData();

		if (sp && sp->getName() == "paddle"&& contact->IsTouching() == true)
		{
			Vec2 vec1 = paddleNode->getPosition();
			Vec2 vec2 = paddleNode->convertToWorldSpace(sp->getPosition());

            drawNode->drawSegment(vec1, vec2, 3.0f, Color4F(1, 1, 1, 1));

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

			//sp->setName("deleting paddle");
			//Paddles[sp->getTag()]->scheduleOnce(schedule_selector(Paddle::dealPaddleDeletion),0.05f);
			Paddles[sp->getTag()]->beginDeletion();
			return;
		}

		
		if (sp && (sp->getName() == "brick" || sp->getName() == "wall") && contact->IsTouching() == true) {

			brickForceVec.x =monsterBody->GetPosition().x - collidedBody->GetPosition().x;
			brickForceVec.y =monsterBody->GetPosition().y - collidedBody->GetPosition().y;
			brickForceVec.Normalize();
			monsterBody->ApplyLinearImpulse(brickForceVec, monsterBody->GetWorldCenter(), true);
			oldVec = b2Vec2(monsterBody->GetLinearVelocity().x, monsterBody->GetLinearVelocity().y);
			oldVec.Normalize();
			monsterBody->SetLinearVelocity(b2Vec2(oldVec.x * 10, oldVec.y * 10));

			if (sp->getName() == "brick")
			{
				for (int i = 0; collidedBodies[i] == NULL; i++) 
				{
					collidedBodies[i] = collidedBody;
					break;
				}
				this->scheduleOnce(schedule_selector(PiecesLayer::dealDeletion),0.05f);
			}

		}
	}

}

bool PiecesLayer::onTouchBegan( Touch* touch, Event *event )
{
	monsterBody->SetTransform(b2Vec2(572 / WORLDSCALE, 280 / WORLDSCALE), 0);
	initMonsterBody();

	auto location = touch->getLocation();

	//Vec2 relatedLocation = paddleNode->convertToNodeSpace(touch->getLocation());
	//for (int i = 0;i <12;i++)
	//{
	//	if(paddles[i] && paddles[i]->getBoundingBox().containsPoint(relatedLocation))
	//	{
	//		if(!m_bMouseDown){
	//			m_bMouseDown=true;
	//			m_vec2_1->updatefunc(location.x ,location.y);
	//			m_nAngle=0;
	//			return true;
	//		}
	//	}
	//}

	if(!m_bMouseDown){
		m_bMouseDown=true;
		m_vec2_1->updatefunc(location.x ,location.y);
		m_nAngle=0;
	}
	return true;
}

void PiecesLayer::onTouchMoved( Touch* touch, Event *event )
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

	//for (int i = 0;i<12;i++)
	//{
	//	if (paddles[i] && paddleBodies[i])
	//	{
	//		Vec2 position = paddleNode->convertToWorldSpace(paddles[i]->getPosition());
	//		b2Vec2 p_position = b2Vec2(position.x / WORLDSCALE, position.y / WORLDSCALE);
	//		float p_angle = -( CC_DEGREES_TO_RADIANS(paddleNode->getRotation() + 150 - 30 * i) );
	//		//b2Body *paddleBody = (b2Body *)paddles[i]->getUserData();
	//		paddleBodies[i]->SetTransform(p_position,p_angle);
	//	}
	//}

	for (int i = 0;i<12;i++)
	{
		Sprite* m_paddle = Paddles[i]->m_paddle;
		
		Vec2 position = paddleNode->convertToWorldSpace(m_paddle->getPosition());
		b2Vec2 p_position = b2Vec2(position.x / WORLDSCALE, position.y / WORLDSCALE);
		float p_angle = -( CC_DEGREES_TO_RADIANS(paddleNode->getRotation() + 150 - 30 * i) );
		//b2Body *paddleBody = (b2Body *)paddles[i]->getUserData();

		if (Paddles[i]->m_paddleBody)
		{
			Paddles[i]->m_paddleBody->SetTransform(p_position,p_angle);
		}
	}

	m_nAngle = 0.0;
}

bool PiecesLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	 //add the background
	auto sprite = Sprite::create("bg.jpg");

	 //position the sprite on the center of the screen
	sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	 //add the sprite as a child to this layer
	this->addChild(sprite, 0);

	initGame();

	return true;
}

void PiecesLayer::dealPaddleDeletion(float dt)
{
	for (int i = 0;i < 12;i++)
	{
		if ( paddles[i] && paddles[i]->getName() == "deleting paddle" && paddleBodies[i])
		{
			//paddleBodies[i]->;
			paddles[i]->setVisible(false);
			world->DestroyBody(paddleBodies[i]);

			//paddleNode->removeChild(paddles[i]);
			//paddles[i] = NULL;
			paddleBodies[i] = NULL;

		}
	}
}

