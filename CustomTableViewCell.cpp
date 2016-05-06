#include "CustomTableViewCell.h"

USING_NS_CC;

void CustomTableViewCell::draw(cocos2d::Renderer *renderer,
	const cocos2d::Mat4 &transform, uint32_t flags)
{
	TableViewCell::draw(renderer, transform, flags);
}