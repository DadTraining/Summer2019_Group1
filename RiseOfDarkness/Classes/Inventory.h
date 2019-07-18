#pragma once
#include "Item.h"
#include "cocos2d.h"
#include "ui\CocosGUI.h"
class Inventory
{
public:
	Inventory(cocos2d::Sprite*);
	~Inventory();
	void Init(cocos2d::Sprite* sprite);
	void AddItem(int);
	void SellItem(int);
	void UseItem(int);
	void SetCapacity(int);
	int GetCapacity();
	void SetSpritePosition(cocos2d::Vec2 pos);
	cocos2d::Vec2 GetSpritePosition();
	void AddToLayer(cocos2d::Layer*);
	cocos2d::ui::Layout *GetTab(int tabIndex);
	std::vector<Item*> GetItems();
	cocos2d::Vec2 GetSize();
private:
	cocos2d::Sprite* mSprite;
	cocos2d::ui::TabControl *tab;
	cocos2d::ui::TabHeader *weapon, *potion, *armor;
	cocos2d::ui::Layout *container1, *container2, *container3;
	int capacity;
	std::vector<Item*> items;
};
