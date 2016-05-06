#ifndef __CUSTOMABLEVIEWCELL_H__
#define __CUSTOMABLEVIEWCELL_H__

#include "cocos2d.h"
#include "extensions\/cocos-ext.h"

class CustomTableViewCell : public cocos2d::extension::TableViewCell
{
public:
	virtual void draw(cocos2d::Renderer *renderer,
		const cocos2d::Mat4 &transform, uint32_t flags) override;
};

#endif // __HELLOWORLD_SCENE_H__
