#pragma once
#include "Item.h"
#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "ItemDatabase.h"
class Inventory
{
public:
	Inventory(cocos2d::Sprite*);
	~Inventory();
	void Init(cocos2d::Sprite* sprite);
	void AddItem(int);  //, cocos2d::Layer*
	void SellItem(int);
	void RemoveItem(int);
	void SetCapacity(int);
	int GetCapacity();
	void SetVisible(bool b);
	bool IsVisible();
	void SetSpritePosition(cocos2d::Vec2 pos);
	cocos2d::Vec2 GetSpritePosition();
	void AddToLayer(cocos2d::Layer*);
	cocos2d::ui::Layout *GetTab(int tabIndex);
	std::vector<Item*> GetItems();
	cocos2d::Vec2 GetSize();
	std::vector<Item*> inventory;
	std::vector<Item*> slots;
	cocos2d::Sprite* clickBox;
	bool InventoryContains(int id);
	void ItemClick(cocos2d::Ref * pSender, int id);  //, cocos2d::Layer*
	int slotX, slotY;
	void btnBackInventory(cocos2d::Ref*);
	void btnEquipInventory(cocos2d::Ref*, int);
	cocos2d::Menu *menu;
private:
	cocos2d::Sprite* mSprite;
	cocos2d::ui::TabControl *tab;
	cocos2d::ui::TabHeader *weapon, *potion, *armor;
	cocos2d::ui::Layout *container1, *container2, *container3;
	int capacity;
	ItemDatabase *database;
	bool showInventory;
	bool showTooltip;
	std::string toolTip;
	int count = 0;
	std::vector<int> targetID;
};
