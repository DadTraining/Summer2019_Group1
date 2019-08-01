#pragma once
#include "cocos2d.h"
#include "ui\CocosGUI.h"

enum class ItemType
{
	weapon,
	potion,
	armor,
	boots,
	arrow
};
enum class WeaponType
{
	sword,
	bow,
	arrow,
	boots,
	armor,
	other
};
class Item {
private:
	std::string itemName;
	int itemID;
	std::string itemDesc;
	int itemPower;
	int sellCost;
	cocos2d::ui::Button* itemIcon;
	ItemType itemType;
	WeaponType weaponType;
	bool itemStackable;
public:
	Item(std::string name, int id, std::string desc, int power, int sellCost, ItemType type, WeaponType wType);
	Item(std::string name);
	cocos2d::ui::Button* GetIcon();
	int GetID();
	
	bool IsStackable();
	Item(const Item*);
	std::string GetDesc();
	ItemType GetType();
	WeaponType GetWeaponType();
	void CreateSprite(int);
	void AddToScene(cocos2d::ui::Layout *layout, cocos2d::Vec2 pos);
	void RemoveFromScene();
	int GetSellCost();
	Item();
	~Item();
	Item(std::string name, int id, std::string desc, int power, int sellCost, ItemType type);
};
