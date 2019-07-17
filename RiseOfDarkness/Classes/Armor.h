#pragma once
#include "Item.h"

class Armor: public Item {
private:
	int defense;
public:
	Armor();
	~Armor();
};
