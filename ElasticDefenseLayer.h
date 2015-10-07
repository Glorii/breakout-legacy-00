#ifndef __helloworld__ElasticDefenseLayer__
#define __helloworld__ElasticDefenseLayer__

#include "ParentOfElastic.h"

#define BOARD_TRACE_RADUIS_MAX 225.0f
#define BOARD_TRACE_RADUIS_MIN 167.5f
#define RHOMBUS_INSIDE_TRACE_RADIUS 27.0f
#define RHOMBUS_WIDTH 37.0f*0.5
#define RHOMBUS_HEIGHT 162.0f*0.5
#define RHOMBUSGROUP_INTERVAL_WIDTH 10.0f
#define RHOMBUSGROUP_INTERVAL_HEIGHT 50.0f
class ElasticDefenseLayer :
	public ParentOfElastic
{
public:
	ElasticDefenseLayer(void);
	~ElasticDefenseLayer(void);

	CREATE_FUNC(ElasticDefenseLayer);
	virtual bool init();

private:
	b2Body* elasticBoard[6];
	float boardDistance;
	float increment;
	virtual void onUpdate();
	virtual void initBrickGroup();
	void initBoardGroup();
	void initSingleRhombusGroup(int i);
	Sprite* addRhombusSprite(float posX,float posY,float rotation);
	void addRhombusBody(Sprite* rhombus);
};


#endif
