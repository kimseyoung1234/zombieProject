#ifndef __INTRO_SCENE_H__
#define __INTRO_SCENE_H__

#include "cocos2d.h"


using namespace cocos2d;
class Intro : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(Intro);

	void doReplaceScene();

	cocos2d::Size winSize;

	void playbtn(Ref * pSender);
	void doRain();
};

#endif // __HELLOWORLD_SCENE_H__