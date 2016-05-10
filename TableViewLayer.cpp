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
	tableView = TableView::create(this, Size(500, 200));
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
		int preCheckIndex = PlayerInfoSingleTon::getInstance()->weaponSeleted;

		// 전에 선택되었던 체크박스 제거하기 위한 처리
		auto pre_cell = table->cellAtIndex(preCheckIndex);

		if (pre_cell) {
			auto pre_check = pre_cell->getChildByTag(123);
			pre_check->setVisible(false);
		}

		// 선택되었다면 체크박스 보이게
		auto check = (Sprite *)cell->getChildByTag(123);
		check->setVisible(true);
	

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
	return Size(200, 200);
}

TableViewCell* TableViewLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
	cocos2d::String *str;
	if (tag == WEAPON)
	{
		 str = String::createWithFormat("gun%02d.png", idx + 1);
	}
	else if (tag == TRAP)
	{
		 str = String::createWithFormat("bomb.png", idx + 1);
	}
	else if (tag == HELPER)
	{
		 str = String::createWithFormat("helper1.png", idx + 1);
	}
	//auto str = String::createWithFormat("gun%02d.png", idx + 1);
	//auto str = String::createWithFormat("gun01.png", idx + 1);
	auto sprite = Sprite::create(str->getCString());

	auto selected = Sprite::create("check.png");

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

		auto check = Sprite::create("check.png");
		check->setAnchorPoint(Vec2::ZERO);
		check->setTag(123);
		/*if (idx == 0)
		{
			log("여기옴?");
			check->setVisible(true);
		}*/
		check->setVisible(false);
		cell->addChild(check);

		
	}
	else
	{
		auto sprite1 = (Sprite*)(cell->getChildByTag(150));
		sprite1->setTexture(str->getCString());
		sprite1->setPosition(Vec2(0, 0));

		if (tag == WEAPON) {
			auto check1 = (Sprite*)(cell->getChildByTag(123));
			if (PlayerInfoSingleTon::getInstance()->weaponSeleted == idx)
			{
				check1->setVisible(true);
			}
		}
	}
	return cell;
}
ssize_t TableViewLayer::numberOfCellsInTableView(TableView *table)
{
	return cellCount;
}