#pragma once
#include "Item.h"

class Inventory
{
public:
	Inventory();
	~Inventory();
	void Init();
	void AddItem(int);
	void DropItem(int);
	void UseItem(int);
	void SetCapacity(int);
	int GetCapacity();
private:
	int capacity;
	std::vector<Item*> items;
};
