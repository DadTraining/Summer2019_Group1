#include "Item.h"
#include "Inventory.h"
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
	clickBox = Sprite::create("res/sprites/item/click.png");
	clickBox->retain();
	slotX = 6;
	slotY = 4;
	database = new ItemDatabase();
	database->Init();
	showTooltip = false;
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
	container2->setOpacity(50);
	container2->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	container2->setBackGroundColor(Color3B::BLUE);
	container2->setBackGroundColorOpacity(50);
	container3 = ui::Layout::create();
	container3->setOpacity(50);
	container3->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	container3->setBackGroundColor(Color3B::RED);
	container3->setBackGroundColorOpacity(50);

	tab->insertTab(0, weapon, container1);
	tab->insertTab(1, potion, container2);
	tab->insertTab(2, armor, container3);

	tab->setSelectTab(0);
	CC_SAFE_RETAIN(tab);
	CC_SAFE_RETAIN(container1);
	CC_SAFE_RETAIN(container2);
	CC_SAFE_RETAIN(container3);

	for (int i = 0; i < slotX*slotY; i++)
	{
		inventory.push_back(new Item());
		slots.push_back(new Item());
	}
}

void Inventory::AddItem(int id)  //,Layer* layer
{
	for (int i = 0; i < inventory.size(); i++)
	{
		if (slots[i]->GetIcon() == NULL)
		{
			log("add item %d", id + 1);
			slots[i] = new Item(database->items[id]);
			slots[i]->GetIcon()->addClickEventListener(CC_CALLBACK_1(Inventory::ItemClick, this, i));
			break;
		}
	}
}

void Inventory::SellItem(int)
{
}

void Inventory::RemoveItem(int id)
{
	for (int i = 0; i < inventory.size(); i++)
	{
		if (slots[i]->GetID() == id && slots[i]->GetIcon() != NULL)
		{
			slots[i] = new Item();
			log("removed item %d", i);
			break;
		}
	}
}



void Inventory::SetCapacity(int)
{
}

int Inventory::GetCapacity()
{
	return capacity;
}

void Inventory::SetVisible(bool b)
{
	tab->setVisible(b);
	mSprite->setVisible(b);
}

bool Inventory::IsVisible()
{
	return (tab->isVisible() && mSprite->isVisible());
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
	return slots;
}

cocos2d::Vec2 Inventory::GetSize()
{
	return tab->getContentSize();
}

bool Inventory::InventoryContains(int id)
{
	for (int i = 0; i < inventory.size(); i++)
	{
		log("%d  ", slots[i]->GetID());
	}

	bool result = false;
	for (int i = 0; i < slots.size(); i++)
	{
		result = slots[i]->GetID() == id;
		if (result)
		{
			break;
		}
	}
	return result;
}

void Inventory::ItemClick(cocos2d::Ref *pSender, int id)   //, Layer* layer
{
	log("item %d clicked", id + 1);
	auto btnEquip = MenuItemImage::create("res/sprites/item/btnEquip.png", "res/sprites/item/btnEquip.png",
		CC_CALLBACK_1(Inventory::btnEquipInventory, this, id));
	auto btnDrop = MenuItemImage::create("res/sprites/item/btnDrop.png", "res/sprites/item/btnDrop.png");
	auto btnBack = MenuItemImage::create("res/sprites/item/btnClose.png", "res/sprites/item/btnClose.png",
		CC_CALLBACK_1(Inventory::btnBackInventory, this));

	btnDrop->setScale(0.5);
	btnEquip->setScale(0.5);
	btnDrop->setPositionY(btnEquip->getPositionY() - btnEquip->getContentSize().height / 2);
	menu = Menu::create(btnEquip, btnDrop, btnBack, NULL);
	menu->setAnchorPoint(Vec2(0, 1));
	menu->setVisible(false);

	btnBack->setPositionY(btnDrop->getPositionY() - btnDrop->getContentSize().height / 2);
	btnBack->setScale(0.5);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	log("item %d clicked!", id);
	menu->setVisible(!menu->isVisible());
	menu->setPosition(slots[id]->GetIcon()->getPosition());
}

void Inventory::btnBackInventory(cocos2d::Ref *)
{
}

void Inventory::btnEquipInventory(cocos2d::Ref *, int)
{
}
