#ifndef __helloworld__BlackholeLayer__
#define __helloworld__BlackholeLayer__

#include "ParentOfElastic.h"

#define BLACKHOLE_TRACK_RADIUS CIRCLE_RADIUS*0.5
#define BLACKHOLE_ROTATE_VELCOCITY 0.5   //rotate 1 degree every frame
#define BLACKHOLE_RADIUS 20
#define TRIANGULAR_BRICK_SIDE_LENGTH 32.0f
#define  BLACKHOLE_ENTRY_FILENAME "blackholeEntry.png"
#define  BLACKHOLE_EXIT_FILENAME "blackholeExit.png"
#define BOSS_EYE_RADIUS ((123.0 - 40) * 0.4 * 0.5)
class BlackholeLayer :
	public ParentOfElastic
{
public:
	BlackholeLayer(void);
	~BlackholeLayer(void);

	virtual bool init();
	CREATE_FUNC(BlackholeLayer);

protected:
	Sprite* blackHoleEntry[4];
	Sprite* blackHoleExit[4];
	Sprite* newHole;
	bool ableToAttackBoss;
	float nextDegree;
	int awakedBlackHole;
	Sprite* bossEyeWhite;
	Sprite* bossEyeBlack;
	b2Body* bossBody;
	void addCoupleTriangularBrick(float posX,float posY,float angle);
	void addSingleTriangluarBrick(float posX,float posY,float angle);
	void addBrickLineOnAngle(int lineSequence,float marginScale,int i,float gapScale,float angle);
	void addBrickLine(int lineSequence,float marginScale, int num,float gapScale,float angleOfOutsideBricks);
	virtual void initBrickGroup();
	virtual void onUpdate();
	virtual void onDealCollision(b2Body* collidedBody);
	void randomAppear(float dt);
	void PopBall(float dt);
	void EnableAttackBoss(float dt);
};


#endif 