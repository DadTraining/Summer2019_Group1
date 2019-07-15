#pragma once
#include "cocos2d.h"
#define WEAPON 1
#define ARMOR 2
#define POTION 3
class Item {
protected:
	int id;
	int kind;
	cocos2d::Sprite* itemSprite;
public:
	Item();
	~Item();
};
