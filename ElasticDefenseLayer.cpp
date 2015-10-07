
#include "ElasticDefenseLayer.h"



ElasticDefenseLayer::ElasticDefenseLayer(void)
{
}


ElasticDefenseLayer::~ElasticDefenseLayer(void)
{
}

bool ElasticDefenseLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	// add the background
	auto sprite = Sprite::create("bg_supplementary.jpg");
	//sprite->setScale(0.5f);
	// position the sprite on the center of the screen
	sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);

	initGame();

	return true;
}

void ElasticDefenseLayer::initBrickGroup()
{
	initBoardGroup();

	for (int i= 0; i < 18;i++)
	{
		initSingleRhombusGroup(i);
		//one for three
	}
}

void ElasticDefenseLayer::onUpdate()
{
	if (boardDistance <= BOARD_TRACE_RADUIS_MIN)
	{
		increment = 1;
	}
	if (boardDistance >= BOARD_TRACE_RADUIS_MAX)
	{
		increment = -1;
	}
	boardDistance += increment;

	for (int i = 0;i < 6;i++)
	{
		Sprite* board = (Sprite*)elasticBoard[i]->GetUserData();
		float angle = CC_DEGREES_TO_RADIANS(board->getRotation());
		float posX = visibleSize.width/2 + boardDistance * sin(angle);
		float posY = visibleSize.height/2 + boardDistance * cos(angle);

		elasticBoard[i]->SetTransform(b2Vec2(posX/WORLDSCALE,posY/WORLDSCALE),elasticBoard[i]->GetAngle() );

		board->setPosition(posX,posY);
	}
}

void ElasticDefenseLayer::initBoardGroup()
{
	boardDistance = BOARD_TRACE_RADUIS_MIN;
	for (int i = 0;i < 6;i++)
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

void ElasticDefenseLayer::initSingleRhombusGroup( int i )
{
	float angle = CC_DEGREES_TO_RADIANS(20*i);
	float posX = visibleSize.width/2 + (RHOMBUS_INSIDE_TRACE_RADIUS + RHOMBUS_HEIGHT/2) * sin(angle);
	float posY = visibleSize.height/2 + (RHOMBUS_INSIDE_TRACE_RADIUS + RHOMBUS_HEIGHT/2) * cos(angle);
	addRhombusBody(addRhombusSprite(posX,posY,20 * i));

	float posX1 = posX + RHOMBUSGROUP_INTERVAL_HEIGHT * sin(angle) - RHOMBUSGROUP_INTERVAL_WIDTH * cos(angle); 
	float posY1 = posY + RHOMBUSGROUP_INTERVAL_HEIGHT * cos(angle) + RHOMBUSGROUP_INTERVAL_WIDTH * sin(angle);
	addRhombusBody(addRhombusSprite(posX1,posY1,20 * i));

	float posX2 = posX + RHOMBUSGROUP_INTERVAL_HEIGHT * sin(angle) + RHOMBUSGROUP_INTERVAL_WIDTH * cos(angle); 
	float posY2 = posY + RHOMBUSGROUP_INTERVAL_HEIGHT * cos(angle) - RHOMBUSGROUP_INTERVAL_WIDTH * sin(angle);
	addRhombusBody(addRhombusSprite(posX2,posY2,20 * i));
}

Sprite* ElasticDefenseLayer::addRhombusSprite( float posX,float posY,float rotation )
{
	auto rhombus = Sprite::create("rhombus.png");
	rhombus->setScale(0.5f);
	Size rhombusSize = rhombus->getContentSize();
	rhombus->setName("brick");
	rhombus->setRotation(rotation);
	rhombus->setTag(1);
	rhombus->setPosition(posX,posY);
	addChild(rhombus);
	return rhombus;
}

void ElasticDefenseLayer::addRhombusBody( Sprite* rhombus )
{
	Size rhombusSize = rhombus->getContentSize();

	b2BodyDef rhombusBodyDef;
	rhombusBodyDef.type = b2_staticBody;
	rhombusBodyDef.position = b2Vec2(rhombus->getPosition().x/WORLDSCALE,rhombus->getPosition().y/WORLDSCALE);
	rhombusBodyDef.angle =  CC_DEGREES_TO_RADIANS(-rhombus->getRotation());

	b2Vec2 points[4];
	points[0].Set(0.0f , RHOMBUS_HEIGHT/2/WORLDSCALE);
	points[1].Set(-RHOMBUS_WIDTH/2 / WORLDSCALE , 0.0f);
	points[2].Set(0.0f , -RHOMBUS_HEIGHT/2/WORLDSCALE);
	points[3].Set(RHOMBUS_WIDTH/2 / WORLDSCALE , 0.0f);
	b2PolygonShape rhombusShape;
	rhombusShape.Set(points,4);

	b2FixtureDef rhombusFixDef;
	rhombusFixDef.shape = &rhombusShape;
	rhombusFixDef.density = 10.0f;
	rhombusFixDef.friction = 0.3;
	rhombusFixDef.restitution = 0.2f;

	b2Body *rhombusBody = world->CreateBody(&rhombusBodyDef);
	rhombusBody->CreateFixture(&rhombusFixDef);

	rhombusBody->SetUserData(rhombus);
}




