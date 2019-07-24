#include "Item.h"
#include "Inventory.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace std;
Inventory::Inventory(cocos2d::Sprite* sprite)
{
	Init(sprite);
	btnUse->removeFromParent();
	btnUse->setPosition(Vec2(btnUse->getContentSize().width / 2 - btnUse->getContentSize().width / 4,
		0 - btnUse->getContentSize().height / 4));
	btnSell->setPosition(Vec2(btnUse->getPositionX() + btnUse->getContentSize().width / 2, btnUse->getPositionY()));
	GetTab(1)->addChild(btnUse, 99);
	GetTab(1)->addChild(btnSell, 99);
}

Inventory::~Inventory()
{
}

void Inventory::Init(cocos2d::Sprite* sprite)
{
	clickBox = Sprite::create("res/sprites/item/click.png");
	btnUse = ui::Button::create("res/sprites/item/buttonUse1.png", "res/sprites/item/buttonUse.png");
	btnUse->setScale(0.5);
	btnSell = ui::Button::create("res/sprites/item/buttonSell1.png", "res/sprites/item/buttonSell.png");
	btnSell->setScale(0.5);

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
	container2 = ui::Layout::create();
	container2->setOpacity(50);
	container3 = ui::Layout::create();
	container3->setOpacity(50);

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
		if (slots[i]->GetID() == id && slots[i]->GetIcon() != NULL && i==id)
		{
			itemAmount[i]--;
			if (itemAmount[i]==0)
			{
				amountLabels[i]->setString("");
				GetTab(1)->removeChild(slots[i]->GetIcon());
				targetID = -1;
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

cocos2d::Sprite * Inventory::GetClickBox()
{
	return clickBox;
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
	targetID = id;
	if (targetID >= 0)
	{
		btnUse->addClickEventListener(CC_CALLBACK_1(Inventory::btnEquipInventory, this));
	}
	
}

void Inventory::btnBackInventory(cocos2d::Ref *pSender)
{
}

void Inventory::btnEquipInventory(cocos2d::Ref *pSender)
{
	log("equip item");
	if (targetID>=0)
	{
		RemoveItem(slots[targetID]->GetID(), targetID);
	}
}
