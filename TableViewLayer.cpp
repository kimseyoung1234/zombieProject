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
	tableView = TableView::create(this, Size(820, 120));
	tableView->setDirection(ScrollView::Direction::HORIZONTAL);
	tableView->setPosition(position);
	tableView->setDelegate(this);
	tableView->setTag(tag);

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

}
Size TableViewLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(190, 170);
}

TableViewCell* TableViewLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
	// 아이템 품목
	cocos2d::String *str;
	// 아이템 가격
	cocos2d::String *price;
	if (tag == WEAPON)
	{
		 str = String::createWithFormat("item/gun%02d.png", idx + 1);
		 if (idx == 0){
			 price = String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->machine_price);
		 }
		 else if (idx == 1) {
			 price = String::createWithFormat(" %d", PlayerInfoSingleTon::getInstance()->ak_price);
		 }
		 else if( idx == 2) {
			 price = String::createWithFormat(" %d", PlayerInfoSingleTon::getInstance()->sniper_price);
		 }
	}
	else if (tag == TRAP)
	{
		 str = String::createWithFormat("item/trap%02d.png", idx + 1);
		 if (idx == 0) {
			 price = String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->trap1_price);
		 }
		 else if (idx == 1)	{
			 price = String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->trap2_price);
		 }
	}
	else if (tag == HELPER)
	{
		 str = String::createWithFormat("item/helper%02d.png", idx + 1);
		 if (idx == 0) {
			 price = String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->helper1_price);
		 }
		 else if (idx == 1)	{
			 price = String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->helper2_price);
		 }
	}

	auto sprite = Sprite::create(str->getCString());

	TableViewCell *cell = table->dequeueCell();
	if (!cell)
	{
		
		cell = new CustomTableViewCell();
		cell->autorelease();
		// 셀 배경
		auto layer_cell = Sprite::create("layer_cell.png");
		layer_cell->setTag(1000);
		layer_cell->setPosition(Vec2(0, 8));
		layer_cell->setAnchorPoint(Vec2::ZERO);
		cell->addChild(layer_cell);

		// 가격
		auto label = LabelBMFont::create(price->getCString(), "fonts/futura-48.fnt");
		label->setPosition(Vec2(75,25));
		label->setScale(0.8f);
		label->setAnchorPoint(Vec2(0.5,0.5));
		label->setTag(101);
		cell->addChild(label,1);
		
		// 품목
		auto sprite = Sprite::create(str->getCString());

		sprite->setPosition(Vec2(79, 60));
		sprite->setTag(150);
		cell->addChild(sprite);

		// 체크박스
		auto check = Sprite::create("check.png");
		check->setAnchorPoint(Vec2::ZERO);
		check->setTag(123);

		check->setVisible(false);
		cell->addChild(check);



		if (tag == WEAPON) {
			if (PlayerInfoSingleTon::getInstance()->weaponSeleted == idx)
			{
				check->setVisible(true);
			}
		}
	}
	else
	{	
		auto sprite1 = (Sprite*)(cell->getChildByTag(150));
		sprite1->setTexture(str->getCString());
		sprite1->setPosition(Vec2(79, 60));

		auto label = (LabelBMFont*)cell->getChildByTag(101);
		label->setString(price->getCString());
		
		if (tag == WEAPON) {
			auto check1 = (Sprite*)(cell->getChildByTag(123));
			if (PlayerInfoSingleTon::getInstance()->weaponSeleted == idx)
			{
				check1->setVisible(true);
			}
			else
			{
				check1->setVisible(false);
			}
		}
	}
	return cell;
}
ssize_t TableViewLayer::numberOfCellsInTableView(TableView *table)
{
	return cellCount;
}