#include "TableViewLayer.h"

USING_NS_CC;


TableViewLayer::TableViewLayer()
{
	log("레이어추가");
	//LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, winSize.height);
	//TableViewLayer::create();
}

bool TableViewLayer::init()
{

	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}
	////////////////////////////////////
	//공용변수들 가져오기
	log("테이블뷰드러옴");
	TableView* tableView1 = TableView::create(this, Size(500, 60));
	tableView1->setDirection(ScrollView::Direction::HORIZONTAL);
	tableView1->setPosition(Vec2(600, 480));

	tableView1->setDelegate(this);
	tableView1->setTag(100);
	this->addChild(tableView1);
	tableView1->reloadData();

	return true;
}

void TableViewLayer::scrollViewDidScroll(ScrollView *view)
{

}
void TableViewLayer::scrollViewDidZoom(ScrollView *view)
{

}
void TableViewLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
	log("Tag : %d\nCell touched at index : %ld", table->getTag(), cell->getIdx());
}
Size TableViewLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	/*	if (idx == 2)
	{
	return Size(100, 100);
	}*/
	return Size(100, 100);
}

TableViewCell* TableViewLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
	auto str = String::createWithFormat("gun%02d.png", idx + 1);
	auto sprite = Sprite::create(str->getCString());

	auto string = String::createWithFormat("%ld", idx);

	TableViewCell *cell = table->dequeueCell();
	if (!cell)
	{
		cell = new CustomTableViewCell();
		cell->autorelease();
		auto sprite = Sprite::create(str->getCString());
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPosition(Vec2(0, 0));
		sprite->setTag(150);
		cell->addChild(sprite);

		auto label = LabelTTF::create(string->getCString(), "Helvetica", 20.0);
		label->setPosition(Vec2::ZERO);
		label->setAnchorPoint(Vec2::ZERO);
		label->setTag(123);
		cell->addChild(label);
	}
	else
	{
		auto sprite1 = (Sprite*)(cell->getChildByTag(150));
		sprite1->setTexture(str->getCString());
		sprite1->setPosition(Vec2(0, 0));

		log("index2 %d", idx);
		auto label = (LabelTTF*)cell->getChildByTag(123);
		label->setString(string->getCString());
	}
	return cell;
}
ssize_t TableViewLayer::numberOfCellsInTableView(TableView *table)
{
	return 2;
}