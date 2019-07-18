#include "Potion.h"

Potion::Potion(int id)
{
	this->id = id;
	kind = POTION;
	amount = 1;
	CreateSprite(id);
}

Potion::~Potion()
{
}
