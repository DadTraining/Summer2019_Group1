#pragma once
#include "Item.h"
#include "cocos2d.h"
class Inventory
{
public:
	Inventory(cocos2d::Sprite*);
	~Inventory();
	void Init();
	void AddItem(int);
	void DropItem(int);
	void UseItem(int);
	void SetCapacity(int);
	int GetCapacity();
	std::vector<Item*> GetItems();
private:
	int capacity;
	std::vector<Item*> items;
};
