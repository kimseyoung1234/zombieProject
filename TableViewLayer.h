#ifndef __TableViewLayer_H__
#define __TableViewLayer_H__

#define WEAPON 500
#define TRAP 600
#define HELPER 700

#include "cocos2d.h"
#include "extensions\/cocos-ext.h"
#include "CustomTableViewCell.h"
#include "Trap.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class TableViewLayer : public cocos2d::LayerColor
	, public cocos2d::extension::TableViewDataSource
	, public cocos2d::extension::TableViewDelegate
{
public:
	TableViewLayer(Vec2 position, int cellcount,int tag);

	Size winSize;
	int cellCount;
	int tag;
	TableView * tableView;
	
	cocos2d::LayerColor* gameLayer;
	std::vector<Trap *>* traps;

	TableView* getTableView();

	virtual void scrollViewDidScroll(ScrollView *view);
	virtual void scrollViewDidZoom(ScrollView *view);
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);

};

#endif // __HELLOWORLD_SCENE_H__
