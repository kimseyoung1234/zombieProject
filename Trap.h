#ifndef __TRAP__H__
#define __TRAP__H__

#include "cocos2d.h"

class Trap : public cocos2d::Sprite {
public:

	Trap(cocos2d::Vec2 position,int type);

	cocos2d::Sprite * sprite;
	int type;
	cocos2d::LayerColor* gameLayer;

	virtual void onEnter();
	virtual void onExit();

};

#endif /* defined(__SpriteExtendEx_Monster__)*/