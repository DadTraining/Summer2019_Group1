#pragma once
#include "Item.h"

class Weapon : public Item {
private:
	int attack;
public:
	Weapon();
	~Weapon();
};
