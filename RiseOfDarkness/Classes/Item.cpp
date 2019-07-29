#include "Item.h"

ItemType Item::GetType()
{
	return itemType;
}

void Item::CreateSprite(int id)
{
}

void Item::AddToScene(cocos2d::ui::Layout *layout, cocos2d::Vec2 pos)
{
}

void Item::RemoveFromScene()
{
	
}

int Item::GetSellCost()
{
	return sellCost;
}

Item::Item()
{
	itemID = NULL;
	itemStackable = false;
	itemIcon = nullptr;
	weaponType = WeaponType::other;
}

Item::~Item()
{
}

Item::Item(std::string name, int id, std::string desc, int power, int Cost, ItemType type)
{
	itemID = id;
	itemName = name;
	itemDesc = desc;
	itemPower = power;
	sellCost = Cost;
	itemType = type;
	itemStackable = ((type == ItemType::potion || type== ItemType::arrow )? true : false);
	weaponType = WeaponType::other;
	itemIcon = cocos2d::ui::Button::create("res/" + name + ".png")->Button::clone();
	itemIcon->retain();
}

Item::Item(std::string name, int id, std::string desc, int power, int Cost, ItemType type, WeaponType wType)
{
	itemID = id;
	itemName = name;
	itemDesc = desc;
	itemPower = power;
	sellCost = Cost;
	itemType = type;
	itemStackable = ((type == ItemType::potion || type==ItemType::arrow) ? true : false);
	weaponType = wType;
	itemIcon = cocos2d::ui::Button::create("res/" + name + ".png")->Button::clone();
	itemIcon->retain();
}

Item::Item(std::string name)
{
	itemID = 99;
	itemStackable = false;
	itemIcon = cocos2d::ui::Button::create("res/" + name + ".png")->clone();
	itemIcon->retain();
}

cocos2d::ui::Widget * Item::GetIcon()
{
	return itemIcon;
}

int Item::GetID()
{
	return itemID;
}

bool Item::IsStackable()
{
	return itemStackable;
}

Item::Item(const Item *item)
{
	itemID = item->itemID;
	itemName = item->itemName;
	itemDesc = item->itemDesc;
	itemPower = item->itemPower;
	sellCost = item->sellCost;
	itemType = item->itemType;
	itemStackable = item->itemStackable;
	itemIcon = item->itemIcon->clone();
	itemIcon->retain();
}

std::string Item::GetDesc()
{
	return itemDesc;
}
