#include "HomeScene.h"

USING_NS_CC;

Size visibleSize;
Size tileMapSize;

Scene* HomeScene::CreateScene()
{
	return HomeScene::create();
}

bool HomeScene::init()
{
	if (!Scene::create())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	tileMap = TMXTiledMap::create("res/tiledMaps/home/home.tmx");
	tileMapSize = tileMap->getContentSize();
	addChild(tileMap, 0);

	mainCharacter = new MainCharacter(this);
	mainCharacter->GetSprite()->setPosition(visibleSize / 2);

	auto frameButton = Sprite::create("res/buttons/frameButton.png");
	auto frameButtonSize = frameButton->getBoundingBox().size;
	frameButton->setPosition(Vec2(frameButtonSize.width, frameButtonSize.height));
	auto frameButtonPosition = frameButton->getPosition();
	frameButton->setVisible(false);
	addChild(frameButton, 3);

	upButton = ui::Button::create("res/buttons/upButtonNormal.png", "res/buttons/upButtonPressed.png");
	upButton->setAnchorPoint(Vec2(0.5, 1));
	upButton->setPosition(Vec2(frameButtonPosition.x, frameButtonPosition.y + frameButtonSize.height / 2));
	addChild(upButton, 4);

	downButton = ui::Button::create("res/buttons/downButtonNormal.png", "res/buttons/downButtonPressed.png");
	downButton->setAnchorPoint(Vec2(0.5, 0));
	downButton->setPosition(Vec2(frameButtonPosition.x, frameButtonPosition.y - frameButtonSize.height / 2));
	addChild(downButton, 5);

	leftButton = ui::Button::create("res/buttons/leftButtonNormal.png", "res/buttons/leftButtonPressed.png");
	leftButton->setAnchorPoint(Vec2(0, 0.5));
	leftButton->setPosition(Vec2(frameButtonPosition.x - frameButtonSize.width / 2, frameButtonPosition.y));
	addChild(leftButton, 6);

	rightButton = ui::Button::create("res/buttons/rightButtonNormal.png", "res/buttons/rightButtonPressed.png");
	rightButton->setAnchorPoint(Vec2(1, 0.5));
	rightButton->setPosition(Vec2(frameButtonPosition.x + frameButtonSize.width / 2, frameButtonPosition.y));
	addChild(rightButton, 7);

	AddListener();

	scheduleUpdate();

	return true;
}

void HomeScene::update(float deltaTime)
{
	OnButtonHold(deltaTime);
}

void HomeScene::AddListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HomeScene::OnTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HomeScene::OnTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(HomeScene::OnTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	upButton->addTouchEventListener(CC_CALLBACK_2(HomeScene::UpButtonTouched, this));
	downButton->addTouchEventListener(CC_CALLBACK_2(HomeScene::DownButtonTouched, this));
	leftButton->addTouchEventListener(CC_CALLBACK_2(HomeScene::LeftButtonTouched, this));
	rightButton->addTouchEventListener(CC_CALLBACK_2(HomeScene::RightButtonTouched, this));
}

bool HomeScene::OnTouchBegan(Touch* touch, Event* event)
{
	return true;
}

bool HomeScene::OnTouchEnded(Touch* touch, Event* event)
{
	return true;
}

void HomeScene::OnTouchMoved(Touch* touch, Event* event)
{
	Vec2 newPosition = touch->getLocation() - touch->getPreviousLocation() + tileMap->getPosition();
	if (newPosition.x <= 0 && newPosition.x >= visibleSize.width - tileMapSize.width
		&&newPosition.y <= 0 && newPosition.y >= visibleSize.height - tileMapSize.height)
	{
		tileMap->setPosition(newPosition);
	}
}

void HomeScene::OnButtonHold(float deltaTime)
{
	if (std::find(heldButtons.begin(), heldButtons.end(), 1) != heldButtons.end())
	{
		mainCharacter->GetSprite()->setPositionY(mainCharacter->GetSprite()->getPositionY() + 1);
	}
	if (std::find(heldButtons.begin(), heldButtons.end(), 2) != heldButtons.end())
	{
		mainCharacter->GetSprite()->setPositionY(mainCharacter->GetSprite()->getPositionY() - 1);
	}
	if (std::find(heldButtons.begin(), heldButtons.end(), 3) != heldButtons.end())
	{
		mainCharacter->GetSprite()->setPositionX(mainCharacter->GetSprite()->getPositionX() - 1);
	}
	if (std::find(heldButtons.begin(), heldButtons.end(), 4) != heldButtons.end())
	{
		mainCharacter->GetSprite()->setPositionX(mainCharacter->GetSprite()->getPositionX() + 1);
	}

}

void HomeScene::UpButtonTouched(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		heldButtons.push_back(1);
	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		heldButtons.erase(std::remove(heldButtons.begin(), heldButtons.end(), 1), heldButtons.end());
	}
}

void HomeScene::DownButtonTouched(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		heldButtons.push_back(2);
	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		heldButtons.erase(std::remove(heldButtons.begin(), heldButtons.end(), 2), heldButtons.end());
	}
}

void HomeScene::LeftButtonTouched(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		heldButtons.push_back(3);
	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		heldButtons.erase(std::remove(heldButtons.begin(), heldButtons.end(), 3), heldButtons.end());
	}
}

void HomeScene::RightButtonTouched(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		heldButtons.push_back(4);
	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		heldButtons.erase(std::remove(heldButtons.begin(), heldButtons.end(), 4), heldButtons.end());
	}
}

