#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <GLES-Render.h>
#include "Monster.h"
#include "Bullet.h"
#include "ContactListener.h"
#include "Trap.h"
#include "Helper.h"

#define PTM_RATIO 32

#define BULLET 100
#define MONSTER 200
#define WORLD 300
#define BARRICADE 400
using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(HelloWorld);

	// 변수
	cocos2d::Sprite * player;
	std::vector<Monster * >* monsters;
	std::vector<Bullet * >* bullets;
	std::vector<b2Body * >* barricade;
	std::vector<Trap *>* traps;
	b2Body * body;
	cocos2d::LayerColor* gameLayer;
	cocos2d::LayerColor* menuLayer;
	cocos2d::Label * levelLabel;
	cocos2d::Label * item_Label;
	cocos2d::Label * item2_Label;


	//cocos2d::ProgressTimer * progress;
	//cocos2d::ProgressTimer * progress2;

	cocos2d::Sprite * waveProgress;
	cocos2d::Sprite * playerHpBar;
	cocos2d::Sprite * playerHp;

	ContactListener* myContactListener;

	cocos2d::Size winSize;
	cocos2d::Texture2D* texture;
	b2World* _world;
	//For debugginh
	GLESDebugDraw* m_debugDraw;

	cocos2d::Sprite * selectedTrap;
	
	Vec2 attackPoint;
	float cocosAngle;
	float attackDelayTime = 0.3;
	float skill1DelayTime = 5.0;
	float skill2DelayTime = 5.0;
	bool isAttack = false;
	bool isWave = false;
	bool isgameOver = false;
	bool isSelectedTrap = false;

	float attackRate = 0;
	//cocos2d::LayerColor* shopLayer;
	////////////////////////

	bool createBox2dWorld(bool debug);
	~HelloWorld();
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	void tick(float dt);
	
	void applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower);
	void waveStart(Ref* pSender);
	void removeObject();
	void gameOver();
	void shopOpen(Ref * pSender);
	void addMenu();
	void remove_anim(Node* sender);
	void cooldown_finish(Node* sender,int type);

	//실험
	Sprite* skill;
	Sprite* skill2;
	Sprite* item;
	Sprite* item2;
	bool isSkill = false;
	bool isSkill2 = false;
	bool skill_cool = false;
	bool skill2_cool = false;
	DrawNode * draw_node;
protected:
	void onDraw(const cocos2d::Mat4& transform, uint32_t flags);
	cocos2d::CustomCommand _customCommand;

};

#endif // __HELLOWORLD_SCENE_H__
