#include "MainMenuScene.h"
#include "ui/CocosGUI.h"
#include "SelectScene.h"
#include "ResourceManager.h"

USING_NS_CC;
using namespace ui;


Scene* MainMenuScene::CreateScene()
{
	return MainMenuScene::create();
}

bool MainMenuScene::init()
{
	if (!Scene::init())
	{
		return false;
	}


	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto bg = ResourceManager::GetInstance()->GetSpriteById(5);//Sprite::create("/res/sprites/bgbg.png");
	bg->removeFromParent();
	bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, -1);

	/*auto labelstart = Label::createWithTTF("Rise of Darkness", "fonts/Marker Felt.ttf", 40);
	
	labelstart->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 1.2 - labelstart->getContentSize().height));
	labelstart->enableOutline(Color4B::RED, 1);
	this->addChild(labelstart, 1);*/
	

	auto button = ResourceManager::GetInstance()->GetButtonById(2);//Button::create("/res/buttons/startt.png");
	button->removeFromParent();
	button->setPosition(Vec2(visibleSize.width / 1.5 + origin.x, visibleSize.height / 2 + origin.y));
	button->setScaleX(2);
	button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			auto gotoNext = CallFunc::create([]() {
				Director::getInstance()->replaceScene(SelectScene::create());
			});
			runAction(gotoNext);
		}
	});

	this->addChild(button);

	auto button2 = ResourceManager::GetInstance()->GetButtonById(1);//Button::create("/res/buttons/option.png");
	button2->removeFromParent();
	button2->setPosition(Vec2(visibleSize.width / 1.5 + origin.x, visibleSize.height / 3 + origin.y));
	button2->setScaleX(2);
	this->addChild(button2);

	auto button3 = ResourceManager::GetInstance()->GetButtonById(0);//Button::create("/res/buttons/ex.png");
	button3->removeFromParent();;
	button3->setPosition(Vec2(visibleSize.width / 1.5 + origin.x, visibleSize.height / 6 + origin.y));
	button3->setScaleX(2);
	button3->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			auto gotoNext = CallFunc::create([]() {
				Director::getInstance()->end();
			});
			runAction(gotoNext);
		}
	});	

	this->addChild(button3);


	return true;
}


