#include "Item.h"

int Item::GetKind()
{
	return kind;
}

void Item::CreateSprite(int id)
{
	if (kind==POTION)
	{
		std::string str = "res/sprites/item/potion_" +
			(id < 10 ? "0" + std::to_string(id) : std::to_string(id))
			+ ".png";
		itemButton = cocos2d::ui::Button::create(str);
		itemButton->setScale(0.8);
		//itemButton->setAnchorPoint(cocos2d::Vec2(0, 1));
		itemButton->retain();
	}
}

void Item::AddToScene(cocos2d::ui::Layout *layout, cocos2d::Vec2 pos)
{
	itemButton->setPosition(pos);
	layout->addChild(itemButton,21);
}

Item::Item()
{
}

Item::~Item()
{
}
