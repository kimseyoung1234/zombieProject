#ifndef __SHOP_SCENE_H__
#define __SHOP_SCENE_H__

#include "cocos2d.h"
#include "extensions\/cocos-ext.h"
#include "CustomTableViewCell.h"
#include "TableViewLayer.h"
#include "Helper.h"
using namespace cocos2d;
using namespace cocos2d::extension;

class ShopScene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(ShopScene);


	Size winSize;

	cocos2d::LayerColor* gameLayer;
	std::vector<Trap *>* traps;
	std::vector<Helper *>* helpers;

	TableViewLayer * weapon_tableLayer;
	TableViewLayer * trap_tableLayer;
	TableViewLayer * helper_tableLayer;


	void shopClose(Ref * pSender);
	void upgrade(Ref * pSender);
	void buy(Ref * pSender);
	void tick(float dt);
	void onEnter();
	void onExit();


};

#endif // __HELLOWORLD_SCENE_H__
