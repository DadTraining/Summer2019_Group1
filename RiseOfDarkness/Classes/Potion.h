#pragma once
#include "Item.h"

class Potion : public Item{
private:
	int hp_regen;
	int mp_regen;
public:
	Potion(int);
	~Potion();
};
