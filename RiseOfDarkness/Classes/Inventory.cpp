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
	clickBox->setPosition(-500, -500);
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

	tab->setSelectTab(1);
	CC_SAFE_RETAIN(tab);
	CC_SAFE_RETAIN(container1);
	CC_SAFE_RETAIN(container2);
	CC_SAFE_RETAIN(container3);

	for (int i = 0; i < slotX*slotY; i++)
	{
		inventory.push_back(new Item());
		slots.push_back(new Item());
		itemAmount.push_back(0);
		auto label = Label::createWithTTF("", "fonts/Marker Felt.ttf", 14);
		label->retain();
		amountLabels.push_back(label);
	}
}

void Inventory::AddItem(int id)
{
	if (InventoryContains(id))
	{
		StackItem(id);
	}
	else
	{
		for (int i = 0; i < inventory.size(); i++)
		{
			if (slots[i]->GetIcon() == NULL)
			{
				log("add item %d", id + 1);
				slots[i] = new Item(database->items[id]);
				slots[i]->GetIcon()->addClickEventListener(CC_CALLBACK_1(Inventory::ItemClick, this, i));
				itemAmount[i]++;
				break;
			}
		}
	}
}

void Inventory::SellItem(int)
{
}

void Inventory::RemoveItem(int id,int index)
{
	for (int i = 0; i < inventory.size(); i++)
	{
		if (slots[i]->GetID() == id && slots[i]->GetIcon() != NULL && i==index)
		{
			itemAmount[i]--;
			if (itemAmount[i]==0)
			{
				amountLabels[i]->setString("");
				GetTab(1)->removeChild(slots[i]->GetIcon());
				slots[i] = new Item();
			}
			else
			{
				amountLabels[i]->setString(to_string(itemAmount[i]));
			}
			log("removed item %d", i);
			break;
		}
	}
}

void Inventory::SetCapacity(int)
{
}

cocos2d::Sprite * Inventory::GetClickBox()
{
	return clickBox;
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
	mSprite->removeFromParent();
	tab->removeFromParent();
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

std::vector<int> Inventory::GetItemAmount()
{
	return itemAmount;
}

std::vector<Label*> Inventory::GetAmountLabel()
{
	return amountLabels;
}

cocos2d::Vec2 Inventory::GetSize()
{
	return tab->getContentSize();
}

void Inventory::StackItem(int id)
{
	for (int i = 0; i < slots.size(); i++)
	{
		if (slots[i]->GetID() == id)
		{
			itemAmount[i]++;
			break;
		}
	}
}

void Inventory::AutoArrange()
{
	log("arrange");
	for (int i = 0; i < slots.size()-1; i++)
	{
		if (slots[i]->GetIcon()==NULL)
		{
			for (int j = i+1; j < slots.size(); j++)
			{
				if (slots[j]->GetIcon()!=NULL)
				{
					swap(slots[i], slots[j]);
					swap(itemAmount[i], itemAmount[j]);
					break;
				}
			}
		}
	}
}

bool Inventory::InventoryContains(int id)
{
	bool result = false;
	for (int i = 0; i < slots.size(); i++)
	{
		result = (slots[i]->GetID() == id && slots[i]->GetIcon()!=NULL);
		if (result)
		{
			break;
		}
	}
	return result;
}

void Inventory::ItemClick(cocos2d::Ref *pSender, int id)   //, Layer* layer
{
	clickBox->setPosition(slots[id]->GetIcon()->getPosition());
	auto btnEquip = MenuItemImage::create("res/sprites/item/btnEquip.png", "res/sprites/item/btnEquip.png",
		CC_CALLBACK_1(Inventory::btnEquipInventory, this, id));
	auto btnDrop = MenuItemImage::create("res/sprites/item/btnDrop.png", "res/sprites/item/btnDrop.png");
	auto btnBack = MenuItemImage::create("res/sprites/item/btnClose.png", "res/sprites/item/btnClose.png",
		CC_CALLBACK_1(Inventory::btnBackInventory, this));
	
	btnDrop->setScale(0.5);
	btnEquip->setScale(0.5);
	btnDrop->setPositionX(btnEquip->getPositionX() + btnEquip->getContentSize().width / 2);
	menu = Menu::create(btnEquip, btnDrop, btnBack, NULL);
	menu->setAnchorPoint(Vec2(0, 1));
	menu->setVisible(false);

	btnBack->setPositionX(btnDrop->getPositionX() + btnDrop->getContentSize().width / 2);
	btnBack->setScale(0.5);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	log("item %d clicked!", id);
	menu->setVisible(!menu->isVisible());
	menu->setPosition(btnEquip->getContentSize().width/2-btnEquip->getContentSize().width/4,
		0-btnEquip->getContentSize().height/4);
	menu->setAnchorPoint(Vec2(0.5, 1));
	menu->removeFromParent();
	GetTab(1)->addChild(menu, 99);
}

void Inventory::btnBackInventory(cocos2d::Ref *)
{
	menu->setVisible(!menu->isVisible());
}

void Inventory::btnEquipInventory(cocos2d::Ref *, int i)
{
	log("equip item");
	RemoveItem(slots[i]->GetID(),i);
	menu->setVisible(!menu->isVisible());
}
