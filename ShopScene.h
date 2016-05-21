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

	bool is_gage1 = false;
	bool is_gage2 = false;
	bool is_gage3 = false;
	Size winSize;

	cocos2d::Sprite * gage;
	cocos2d::LayerColor* gameLayer;
	std::vector<Trap *>* traps;
	std::vector<Helper *>* helpers;

	TableViewLayer * weapon_table;
	TableViewLayer * trap_table;
	TableViewLayer * helper_table;
	LabelBMFont * money_label;

	Sprite * item;
	Sprite * item2;
	LabelBMFont * item_Label;
	LabelBMFont * item2_Label;

	void buyAni(int price);
	void remove_label(Node* sender);
	void remove_sprite(Node* sender);
	void shopClose(Ref * pSender);
	void upgrade(Ref * pSender);
	void buy(Ref * pSender);
	void tick(float dt);
	void onEnter();
	void onExit();


};

#endif // __HELLOWORLD_SCENE_H__
