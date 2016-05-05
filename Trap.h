#ifndef __TRAP__H__
#define __TRAP__H__

#include "cocos2d.h"

class Trap : public cocos2d::Sprite {
public:

	Trap(cocos2d::Vec2 position);

	cocos2d::Vec2 position;
	cocos2d::Sprite * sprite;

	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

};

#endif /* defined(__SpriteExtendEx_Monster__)*/