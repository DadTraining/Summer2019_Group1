#include "Item.h"

int Item::GetType()
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

Item::Item()
{
	itemID = NULL;
	itemStackable = false;
	itemIcon = nullptr;
}

Item::~Item()
{
}

Item::Item(std::string name, int id, std::string desc, int power, int speed, ItemType type)
{
	itemID = id;
	itemName = name;
	itemDesc = desc;
	itemPower = power;
	itemSpeed = speed;
	itemType = type;
	itemStackable = (type == potion ? true : false);
	itemIcon = cocos2d::ui::Button::create("res/" + name + ".png")->Button::clone();
	itemIcon->retain();
}

Item::Item(std::string name)
{
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
	itemSpeed = item->itemSpeed;
	itemType = item->itemType;
	itemStackable = item->itemStackable;
	itemIcon = item->itemIcon->clone();
	itemIcon->retain();
}

std::string Item::GetDesc()
{
	return itemDesc;
}
