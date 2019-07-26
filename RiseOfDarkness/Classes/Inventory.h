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
	void RemoveItem(int,int,ItemType);
	cocos2d::Sprite* GetClickBox();
	void SetVisible(bool b);
	bool IsVisible();
	void SetSpritePosition(cocos2d::Vec2 pos);
	cocos2d::Vec2 GetSpritePosition();
	void AddToLayer(cocos2d::Layer*);
	cocos2d::ui::Layout *GetTab(int tabIndex);
	std::vector<Item*> GetItems();
	std::vector<Item*> GetItemsWeapon();
	std::vector<int> GetItemAmount(int);
	
	std::vector<Item*> GetArrows();
	std::vector<cocos2d::Label*> GetAmountLabel(int);
	
	cocos2d::Vec2 GetSize();
	std::vector<Item*> inventory;
	std::vector<Item*> slots;
	std::vector<Item*> weapons;
	std::vector<Item*> arrows;
	int GetIdByIcon(int, ItemType);
	void StackItem(int id, ItemType type);
	void AutoArrange();  //arrange list item after remove an item
	bool InventoryContains(int id,ItemType);
	void ItemClick(cocos2d::Ref * pSender, int id, ItemType);
	void EquipItem(cocos2d::Ref *);
	void btnEquipInventory(cocos2d::Ref *);
	int slotX, slotY;
	int targetID;
private:
	cocos2d::Sprite* clickBox;
	cocos2d::Sprite* mSprite;
	cocos2d::ui::TabControl *tab;
	cocos2d::ui::TabHeader *weapon, *potion, *armor, *boots, *arrow;
	cocos2d::ui::Layout *container1, *container2, *container3, *arrowContainer;
	int capacity;
	ItemDatabase *database;
	bool showInventory;
	bool showTooltip;
	std::string toolTip;
	int count = 0;
	std::vector<int> itemAmount;
	std::vector<int> arrowAmount;
	std::vector<cocos2d::Label*> amountLabels;
	std::vector<cocos2d::Label*> amountArrowLabels;
	cocos2d::Menu* menu;
	cocos2d::ui::Button *btnUse,*btnSell;
};
