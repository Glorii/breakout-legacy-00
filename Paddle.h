#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <math.h>
USING_NS_CC;

#define PIECES_PADDLE_LENGTH 177.0f
#define PIECES_PADDLE_THICKNESS 20.0f
#define CIRCLE_RADIUS 357
#define WORLDSCALE 30

class Paddle:public Node
{
public:
	Paddle(int index,b2World* world,Node* paddleNode);
	~Paddle(void);

	Sprite* m_paddle;
	b2Body* m_paddleBody;

	 void addPaddleSprite();
	 void addPaddleBody();
	
	 void beginDeletion();

private:
	b2World* m_world;
	int m_index;
	Node* m_paddleNode;

	void dealPaddleDeletion(float dt);
	void dealRecoverPaddleBody(float dt);
};

