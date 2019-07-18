#pragma once
#include "cocos2d.h"
#include "ui\CocosGUI.h"
#define WEAPON 1
#define ARMOR 2
#define POTION 3
#define BOOT 4
#define BOW 5
class Item {
protected:
	cocos2d::ui::Button* itemButton;
	int id;
	int kind;
	int amount;
public:
	int GetKind();
	void CreateSprite(int);
	void AddToScene(cocos2d::ui::Layout *layout, cocos2d::Vec2 pos);
	Item();
	~Item();
};
