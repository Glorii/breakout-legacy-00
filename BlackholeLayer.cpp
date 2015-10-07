#include "BlackholeLayer.h"


BlackholeLayer::BlackholeLayer(void)
{
}


BlackholeLayer::~BlackholeLayer(void)
{
}

void BlackholeLayer::initBrickGroup()
{
	ableToAttackBoss =true;

	for (int i = 0;i < 4;i ++)
	{
		blackHoleEntry[i] = Sprite::create(BLACKHOLE_ENTRY_FILENAME);
		blackHoleEntry[i]->setPosition(visibleSize.width/2 + CIRCLE_RADIUS/2,visibleSize.height/2);
		blackHoleEntry[i]->setScale(0.3);
		blackHoleEntry[i]->setTag(1);
		addChild(blackHoleEntry[i]);

		if (i == 1 || i ==3)
		{
			blackHoleEntry[i]->setTag(0);
			blackHoleEntry[i]->setVisible(false);
		}
		
	}

	nextDegree = 0;
	awakedBlackHole = 5;

	//brick
	addCoupleTriangularBrick(visibleSize.width/2,visibleSize.height/2 + 3.0f * TRIANGULAR_BRICK_SIDE_LENGTH * cos(CC_DEGREES_TO_RADIANS(30))  ,0);

	addBrickLine(0,0.5,2,1,0);
	addBrickLine(1, 0,3,1,0);
	
	//addBrickLine(3,0.5,1,1,180);
	addBrickLineOnAngle(2,0,0,1,0);
	addCoupleTriangularBrick(visibleSize.width/2,visibleSize.height/2 + 3.0f * TRIANGULAR_BRICK_SIDE_LENGTH * cos(CC_DEGREES_TO_RADIANS(30))  ,180);
	///////////////boss
	bossEyeWhite = Sprite::create("bossEyeWhite.png");
	bossEyeWhite->setPosition(visibleSize.width/2,visibleSize.height/2);
	bossEyeWhite->setScale(0.4);
	bossEyeWhite->setTag(3);
	bossEyeWhite->setName("boss");
	addChild(bossEyeWhite);

	bossEyeBlack = Sprite::create("bossEyeBlack.png");
	bossEyeBlack->setPosition(visibleSize.width/2,visibleSize.height/2);
	bossEyeBlack->setScale(0.4);
	this->addChild(bossEyeBlack);

	b2BodyDef bossBodyDef;
	bossBodyDef.type = b2_staticBody;

	bossBodyDef.position = b2Vec2( bossEyeWhite->getPosition().x / WORLDSCALE, bossEyeWhite->getPosition().y / WORLDSCALE);
	bossBody = world->CreateBody(&bossBodyDef);
	bossBody->SetUserData(bossEyeWhite);

	b2CircleShape bossShape;
	bossShape.m_radius = 123*0.4/2/WORLDSCALE;

	b2FixtureDef bossFixDef;
	bossFixDef.shape = &bossShape;
	bossFixDef.density = 1.0f;
	bossFixDef.friction = 0.3f;
	bossFixDef.restitution = 0.2f;

	bossBody->CreateFixture(&bossFixDef);
}


void BlackholeLayer::addBrickLineOnAngle( int lineSequence,float marginScale,int i,float gapScale,float angle)
{
	float x1 = visibleSize.width /  2 + (marginScale + 0.5) * TRIANGULAR_BRICK_SIDE_LENGTH +TRIANGULAR_BRICK_SIDE_LENGTH * gapScale * i;
	float x2 = visibleSize.width / 2 - (marginScale + 0.5) * TRIANGULAR_BRICK_SIDE_LENGTH - TRIANGULAR_BRICK_SIDE_LENGTH * gapScale * i;
	float y = visibleSize.height / 2 + TRIANGULAR_BRICK_SIDE_LENGTH * cos(CC_DEGREES_TO_RADIANS(30))  * lineSequence ;
	addCoupleTriangularBrick(x1,y,angle);
	if (x1 != x2)
	{
		addCoupleTriangularBrick(x2,y,angle);
	}
}

void BlackholeLayer::addBrickLine( int lineSequence,float marginScale, int num,float gapScale,float angleOfOutsideBricks )
{
	//auto Brick = Sprite::create(filename);
	//Size brickSize = Brick->getContentSize();
	for (int i = 0;i < num  ;i++)
	{
		addBrickLineOnAngle(lineSequence + 1,marginScale,i,gapScale,180 - angleOfOutsideBricks);

		if (lineSequence == 0 && i == 0)
		{
			continue;
		}

		addBrickLineOnAngle(lineSequence,marginScale - 0.5,i,gapScale,angleOfOutsideBricks);
		
	}
}

void BlackholeLayer::addCoupleTriangularBrick(float posX,float posY,float angle)
{
	addSingleTriangluarBrick(posX,visibleSize.height - posY,angle);
	addSingleTriangluarBrick(posX,posY,180 - angle);
}

void BlackholeLayer::addSingleTriangluarBrick( float posX,float posY,float angle )
{	
	float spPosY =posY - cos(CC_DEGREES_TO_RADIANS(angle)) * TRIANGULAR_BRICK_SIDE_LENGTH * cos(CC_DEGREES_TO_RADIANS(30)) * 0.5f;
	
	Sprite* brick;
	if (Vec2(posX,spPosY).distance(Vec2(visibleSize.width/2,visibleSize.height/2)) < 2 * TRIANGULAR_BRICK_SIDE_LENGTH * cos(CC_DEGREES_TO_RADIANS(30)))
	{
		brick = Sprite::create("TriangleBrickRedTwice.png");
		brick->setRotation(angle);
		brick->setTag(2);
	}else
	{
		brick = Sprite::create("TriangleBrickGreyOnce.png");
		brick->setRotation(180 - angle);
		brick->setTag(1);
	}
	
	Size brickSize = brick->getContentSize();
	brick->setName("brick");
	brick->setPosition(posX,spPosY);
	
	brick->setScale(0.5f);
	addChild(brick);

	b2BodyDef brickBodyDef;
	brickBodyDef.type = b2_staticBody;
	brickBodyDef.angle = CC_DEGREES_TO_RADIANS(angle);

	brickBodyDef.position = b2Vec2( posX / WORLDSCALE, posY / WORLDSCALE);
	b2Body *brickBody = world->CreateBody(&brickBodyDef);
	brickBody->SetUserData(brick);

	b2Vec2 points[3];
	points[0].Set(-0.5f * TRIANGULAR_BRICK_SIDE_LENGTH / WORLDSCALE, 0.0f);
	points[1].Set(0.5f * TRIANGULAR_BRICK_SIDE_LENGTH / WORLDSCALE, 0.0f);
	points[2].Set(0.0f, -1.0f * cos(CC_DEGREES_TO_RADIANS(30)) * TRIANGULAR_BRICK_SIDE_LENGTH / WORLDSCALE);
	b2PolygonShape brickShape;
	brickShape.Set(points,3);


	b2FixtureDef brickFixDef;
	brickFixDef.shape = &brickShape;
	brickFixDef.density = 1.0f;
	brickFixDef.friction = 0.3f;
	brickFixDef.restitution = 0.2f;

	brickBody->CreateFixture(&brickFixDef);
}


bool BlackholeLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	// add the background
	auto sprite = Sprite::create("bgwhite.jpg");
	//sprite->setOpacity();
	// position the sprite on the center of the screen
	sprite->setPosition(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);

	// add the sprite as a child to this layer
	//this->addChild(sprite, 0);

	initGame();

	return true;
}

void BlackholeLayer::onUpdate()
{
	if (bossEyeBlack)
	{
		if (bossEyeWhite->getTag())
		{
			Vec2 dir = monster->getPosition() - Vec2(visibleSize.width/2,visibleSize.height/2);
			dir.normalize();
			bossEyeBlack->setPosition(dir * BOSS_EYE_RADIUS +  Vec2(visibleSize.width/2,visibleSize.height/2));
		}else
		{
			removeChild(bossEyeBlack);
			bossEyeBlack = NULL;
			bossEyeWhite = NULL;
			bossBody  = NULL;
		}
	}
	
	
	//float blackHoleRadius = CIRCLE_RADIUS / 3;
	for (int i = 0;i < 4;i++)
	{
		Sprite* m_blackhole = blackHoleEntry[i];
		if (m_blackhole )
		{
			if (nextDegree>360)
			{
				nextDegree = nextDegree - 360;
			}
			nextDegree += BLACKHOLE_ROTATE_VELCOCITY;
			float posx = cos(CC_DEGREES_TO_RADIANS(nextDegree + i*90))*BLACKHOLE_TRACK_RADIUS + visibleSize.width/2;
			float posy = sin(CC_DEGREES_TO_RADIANS(nextDegree + i*90))*BLACKHOLE_TRACK_RADIUS + visibleSize.height/2;
			m_blackhole->setPosition(posx,posy);
		}
	}

	for (int i = 0;i<4;i++)
	{
		blackHoleExit[i] = NULL;

		Sprite* m_blackhole = blackHoleEntry[i];
		if (m_blackhole->getTag() == 1 && i != awakedBlackHole)
		{
			if (monster->getPosition().distance(m_blackhole->getPosition()) <= BLACKHOLE_RADIUS && awakedBlackHole == 5)
			{
				monsterBody->SetAwake(false);
				monster->setVisible(false);
				//m_blackhole->setVisible(false);
				//this->seq
				awakedBlackHole = i;
				/*DelayTime * delayAction = DelayTime::create(0.5f);  
				CallFunc * callFunc = CallFunc::create(randomAppear);  
				this->runAction(Sequence::createWithTwoActions(delayAction, callFunc));
				this->runAction(Sequence::createWithTwoActions(delayAction,  CallFunc::create(PopBall)));*/
				//this->runAction(Sequence(randomAppear,PopBall))
				this->scheduleOnce(schedule_selector(BlackholeLayer::randomAppear),0.2f);
				this->scheduleOnce(schedule_selector(BlackholeLayer::PopBall),0.5f);
				
				return;
			}
		}
	}
}

void BlackholeLayer::onDealCollision(b2Body* collidedBody)
{
	if (!ableToAttackBoss)
	{
		return;
	}
	Sprite* sp = (Sprite*)collidedBody->GetUserData();

	if (sp &&  sp->getName() == "boss")
	{
		removeJudgment(collidedBody);
		ableToAttackBoss = false;

		this->scheduleOnce(schedule_selector(BlackholeLayer::EnableAttackBoss),0.35f);

		for (int i = 1;i < 4;i  += 2)
		{
			if (blackHoleEntry[i]->getTag() == 0)
			{
				blackHoleEntry[i]->setTag(1);
				blackHoleEntry[i]->setVisible(true);
				return;
			}
			
		}
	}
	

}

void BlackholeLayer::PopBall(float dt)
{
	float randPosX = newHole->getPosition().x;//(rand()%2 - 1)*BLACKHOLE_TRACK_RADIUS + visibleSize.width/2;
	float randPosY = newHole->getPosition().y;//(rand()%2 - 1)*BLACKHOLE_TRACK_RADIUS + visibleSize.height/2;


	removeChild(newHole);
	newHole = NULL;
	//blackHole[awakedBlackHole]->/*setPosition*/(Vec2(randPosX,randPosY));
	monsterBody->SetTransform(b2Vec2(randPosX / WORLDSCALE,randPosY / WORLDSCALE),monsterBody->GetAngle());
	monster->setPosition(randPosX,randPosY);


	//blackHole[awakedBlackHole]->setVisible(true);
	//removeChild(blackHole[awakedBlackHole]);
	//blackHole[awakedBlackHole] = NULL;

	monster->setVisible(true);
	monsterBody->SetAwake(true);


	/*b2Vec2 vDirect = b2Vec2(randPosX - visibleSize.width/2,randPosY - visibleSize.height/2);
	vDirect.Normalize();
	vDirect *= 10;*/
	float angle = rand()%360;
	monsterBody->SetLinearVelocity(b2Vec2(10 * cos(angle),-10 * sin(angle)));

	awakedBlackHole = 5;
	//DelayTime * delayAction = DelayTime::create(dt);  
	//CallFunc * callFunc = CallFunc::create(pSelectorTarget, selector);  
	//this->runAction(Sequence::createWithTwoActions(delayAction, callFunc));

}

void BlackholeLayer::randomAppear( float dt )
{
	float randPosX,randPosY,d;
	do 
	{
		randPosX = (rand()%2 - 1)*BLACKHOLE_TRACK_RADIUS + visibleSize.width/2;
		randPosY = (rand()%2 - 1)*BLACKHOLE_TRACK_RADIUS + visibleSize.height/2;
		d = Vec2(randPosX,randPosY).distance(Vec2(visibleSize.width/2,visibleSize.height/2));
	} while (d < 4 * TRIANGULAR_BRICK_SIDE_LENGTH * cos(CC_DEGREES_TO_RADIANS(30)));
	
	
	newHole = Sprite::create(BLACKHOLE_EXIT_FILENAME);
	newHole->setPosition(randPosX,randPosY);
	newHole->setScale(0.3);
	addChild(newHole);

}

void BlackholeLayer::EnableAttackBoss( float dt )
{
	ableToAttackBoss = true;
}




