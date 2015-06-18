#include "Paddle.h"


Paddle::Paddle(int index,b2World* world,Node* paddleNode)
{
	//m_paddle = paddle;
	//Node::create();
	this->onEnter();
	m_world = world;
	m_paddleNode = paddleNode;
	m_index = index;
	//addPaddleSprite(paddleNode,index);
	//addPaddleBody();
	//return m_paddle;
	addPaddleSprite();
	addPaddleBody();
}


Paddle::~Paddle(void)
{
}

void Paddle::addPaddleSprite()
{
	auto paddle = Sprite::create("30DegreePaddle.png");
	paddle->setName("paddle");
	paddle->setTag(m_index);
	paddle->setRotation(150 - 30 * m_index);
	float posX = 0 - (CIRCLE_RADIUS - PIECES_PADDLE_THICKNESS/4) * cos(CC_DEGREES_TO_RADIANS(15))*sin(CC_DEGREES_TO_RADIANS(paddle->getRotation()));
	float posY = 0 - (CIRCLE_RADIUS - PIECES_PADDLE_THICKNESS/4) * cos(CC_DEGREES_TO_RADIANS(15))*cos(CC_DEGREES_TO_RADIANS(paddle->getRotation()));
	paddle->setPosition(posX,posY);

	m_paddle = paddle;

	m_paddleNode->addChild(m_paddle);
}

void Paddle::addPaddleBody()
{
	b2BodyDef paddleBodyDef;
	paddleBodyDef.type = b2_staticBody;
	//paddleBodyDef.position = b2Vec2((visibleSize.width / 2 - CIRCLE_RADIUS)/ WORLDSCALE, (visibleSize.height / 2) / WORLDSCALE);

	b2PolygonShape paddleShape;
	paddleShape.SetAsBox( PIECES_PADDLE_LENGTH /2 / WORLDSCALE,PIECES_PADDLE_THICKNESS/2 / WORLDSCALE);

	b2FixtureDef paddleFixDef;
	paddleFixDef.shape = &paddleShape;
	paddleFixDef.density = 10.0f;
	paddleFixDef.friction = 0.3;
	paddleFixDef.restitution = 0.2f;

	b2Body *paddleBody = m_world->CreateBody(&paddleBodyDef);
	paddleBody->CreateFixture(&paddleFixDef);

	paddleBody->SetUserData(m_paddle);

	Vec2 position = m_paddleNode->convertToWorldSpace(m_paddle->getPosition());

	paddleBody->SetTransform(b2Vec2(position.x,position.y),CC_DEGREES_TO_RADIANS(m_paddleNode->getRotation() + 150 - 30 * m_index) );

	m_paddleBody = paddleBody;
}

void Paddle::dealRecoverPaddleBody( float dt )
{
		//this->scheduleOnce(schedule_selector(Paddle::dealPaddleDeletion),0.05f);
	m_paddle->setVisible(true);
	addPaddleBody();
}

void Paddle::dealPaddleDeletion( float dt )
{
	/*if (m_paddle && m_paddle->getName() == "deleting paddle" )
	{*/
		//paddleBodies[i]->;
		m_paddle->setVisible(false);
		m_world->DestroyBody(m_paddleBody);

		//paddleNode->removeChild(paddles[i]);
		//paddles[i] = NULL;
		m_paddleBody = NULL;

		//this->scheduleOnce(schedule_selector(Paddle::dealRecoverPaddleBody),1.0f);

	//}
}

void Paddle::beginDeletion()
{
	this->scheduleOnce(schedule_selector(Paddle::dealPaddleDeletion),0.05f);
	this->scheduleOnce(schedule_selector(Paddle::dealRecoverPaddleBody),5.0f);
}
