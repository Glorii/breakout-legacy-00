#ifndef __helloworld__PiecesLayer__
#define __helloworld__PiecesLayer__

#include "ParentOfPieces.h"
#include "Paddle.h"

#define PIECES_PADDLE_LENGTH 177.0f
#define PIECES_PADDLE_THICKNESS 20.0f

class PiecesLayer :
	public ParentOfPieces
{
public:
	PiecesLayer(void);
	~PiecesLayer(void);
	virtual bool init();
	CREATE_FUNC(PiecesLayer);

protected:
	Sprite* paddles[12];//user data for circular paddle
	b2Body* paddleBodies[12];
	//Sprite* toBeRecoverPaddles[12];
	Paddle* Paddles[12];

	 virtual void addPaddle();
	 virtual void calInterPoint();
	 virtual void calReflectedRay();
	 bool calInterPoint(Vec2 linePoint,b2Vec2 directVec,Vec2 segmentVertax1,Vec2 segmentVertax2);
	 void calReflectedRay(Vec2 tobeReflectPoint,Vec2 symmetryAxisVertax1,Vec2 symmetryAxisVertax2);
	 virtual bool onTouchBegan(Touch* touch, Event *event);
	 virtual void  onTouchMoved(Touch* touch, Event *event);
	 virtual void dealCollision(float dur);

	 void dealPaddleDeletion(float dt);
	 void dealRecoverPaddleBody(float dt);
};

#endif 