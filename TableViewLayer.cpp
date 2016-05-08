#include "TableViewLayer.h"
#include "DataSingleTon.h"
#include "PlayerInfoSingleTon.h"
USING_NS_CC;


TableViewLayer::TableViewLayer(Vec2 position,int cellcount,int tag)
{
	traps = DataSingleTon::getInstance()->getTraps();
	gameLayer = DataSingleTon::getInstance()->getGameLayer();
	winSize = Director::getInstance()->getWinSize();

	this->cellCount = cellcount;
	this->tag = tag;
	tableView = TableView::create(this, Size(500, 60));
	tableView->setDirection(ScrollView::Direction::HORIZONTAL);
	tableView->setPosition(position);
	tableView->setDelegate(this);
	tableView->setTag(tag);
	//shopLayer->addChild(tableView);
	tableView->reloadData();

}

TableView* TableViewLayer::getTableView()
{
	return tableView;
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
	int cellIdx = cell->getIdx();
	//스프라이트 변경
	/*auto c = (Sprite * )cell->getChildByTag(150);
	c->setTexture("gun02.png");*/

	if (tag == WEAPON)
	{
		PlayerInfoSingleTon::getInstance()->weaponSeleted = cellIdx;
	
	}
	else if (tag == TRAP)
	{
		PlayerInfoSingleTon::getInstance()->trapSeleted = cellIdx;
		PlayerInfoSingleTon::getInstance()->helperSeleted = -1;
	
	}
	else if (tag == HELPER)
	{
		PlayerInfoSingleTon::getInstance()->helperSeleted = cellIdx;
		PlayerInfoSingleTon::getInstance()->trapSeleted = -1;
	
	}
	/*
	auto trap = new Trap(Vec2(winSize.width / 2, winSize.height / 2), 1);
	gameLayer->addChild(trap);
	traps->push_back(trap);
	*/
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
	//auto str = String::createWithFormat("gun%02d.png", idx + 1);
	auto str = String::createWithFormat("gun01.png", idx + 1);
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
	return cellCount;
}