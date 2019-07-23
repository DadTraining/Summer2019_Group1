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
	void AddItem(int);
	void SellItem(int);
	void RemoveItem(int,int);
	void SetCapacity(int);
	cocos2d::Sprite* GetClickBox();
	int GetCapacity();
	void SetVisible(bool b);
	bool IsVisible();
	void SetSpritePosition(cocos2d::Vec2 pos);
	cocos2d::Vec2 GetSpritePosition();
	void AddToLayer(cocos2d::Layer*);
	cocos2d::ui::Layout *GetTab(int tabIndex);
	std::vector<Item*> GetItems();
	std::vector<int> GetItemAmount();
	std::vector<cocos2d::Label*> GetAmountLabel();
	cocos2d::Vec2 GetSize();
	std::vector<Item*> inventory;
	std::vector<Item*> slots;
	void StackItem(int id);
	void AutoArrange();  //arrange list item after remove an item
	bool InventoryContains(int id);
	void ItemClick(cocos2d::Ref * pSender, int id);  //, cocos2d::Layer*
	int slotX, slotY;
	void btnBackInventory(cocos2d::Ref*);
	void btnEquipInventory(cocos2d::Ref*, int);
	cocos2d::Menu *menu;
private:
	cocos2d::Sprite* clickBox;
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
	std::vector<int> itemAmount;
	std::vector<cocos2d::Label*> amountLabels;
};
