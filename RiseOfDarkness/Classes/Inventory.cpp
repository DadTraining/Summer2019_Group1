#include "Item.h"
#include "Inventory.h"
#include "Armor.h"
#include "Potion.h"
#include "Weapon.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace std;
Inventory::Inventory(cocos2d::Sprite* sprite)
{
	Init(sprite);
}

Inventory::~Inventory()
{
}

void Inventory::Init(cocos2d::Sprite* sprite)
{
	mSprite = sprite;
	tab = ui::TabControl::create();
	tab->setContentSize(Size(384, 325));
	tab->setHeaderHeight(69.f);
	tab->setHeaderWidth(70.f);
	tab->setHeaderSelectedZoom(.1f);
	tab->setHeaderDockPlace(ui::TabControl::Dock::TOP);
	weapon = cocos2d::ui::TabHeader::create();
	weapon->setTitleText("WEAPON");
	potion = cocos2d::ui::TabHeader::create();
	potion->setTitleText("POTION");
	armor = cocos2d::ui::TabHeader::create();
	armor->setTitleText("ARMOR");
	capacity = 24;
	container1 = ui::Layout::create();
	container1->setOpacity(255);
	container1->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	container1->setBackGroundColor(Color3B::GRAY);
	container1->setBackGroundColorOpacity(50);
	container2 = ui::Layout::create();
	container2->setOpacity(255);
	container2->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	container2->setBackGroundColor(Color3B::BLUE);
	container2->setBackGroundColorOpacity(50);
	container3 = ui::Layout::create();
	container3->setOpacity(255);
	container3->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	container3->setBackGroundColor(Color3B::RED);
	container3->setBackGroundColorOpacity(50);

	tab->insertTab(0, weapon, container1);
	tab->insertTab(1, potion, container2);
	tab->insertTab(2, armor, container3);

	tab->setSelectTab(0);
	CC_SAFE_RETAIN(tab);
}

void Inventory::AddItem(int id)
{
	items.push_back(new Potion(id));
}

void Inventory::SellItem(int)
{
}

void Inventory::UseItem(int)
{
}

void Inventory::SetCapacity(int)
{
}

int Inventory::GetCapacity()
{
	return capacity;
}

void Inventory::SetSpritePosition(cocos2d::Vec2 pos)
{
	mSprite->setPosition(pos);
	tab->setPosition(pos);
}

cocos2d::Vec2 Inventory::GetSpritePosition()
{
	return mSprite->getPosition();
}

void Inventory::AddToLayer(cocos2d::Layer *layer)
{
	layer->addChild(mSprite, 16);
	layer->addChild(tab, 17);
}

cocos2d::ui::Layout *Inventory::GetTab(int tabIndex)
{
	switch (tabIndex)
	{
	case 0:
		return container1;
	case 1:
		return container2;
	case 2: 
		return container3;
	default:
		return container3;
		break;
	}
}

std::vector<Item*> Inventory::GetItems()
{
	return items;
}

cocos2d::Vec2 Inventory::GetSize()
{
	return tab->getContentSize();
}
