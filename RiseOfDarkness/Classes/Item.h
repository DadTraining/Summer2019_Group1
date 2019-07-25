#pragma once
#include "cocos2d.h"
#include "ui\CocosGUI.h"

enum ItemType
{
	weapon,
	potion,
	armor
};

class Item {
private:
	std::string itemName;
	int itemID;
	std::string itemDesc;
	int itemPower;
	int itemSpeed;
	cocos2d::ui::Widget* itemIcon;
	ItemType itemType;
	bool itemStackable;
public:
	Item(std::string name, int id, std::string desc, int power, int speed, ItemType type);
	Item(std::string name);
	cocos2d::ui::Widget* GetIcon();
	int GetID();
	bool IsStackable();
	Item(const Item*);
	std::string GetDesc();
	int GetType();
	void CreateSprite(int);
	void AddToScene(cocos2d::ui::Layout *layout, cocos2d::Vec2 pos);
	void RemoveFromScene();
	Item();
	~Item();
};
