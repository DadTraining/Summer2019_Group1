#include "SelectScene.h"
#include "ui/CocosGUI.h"
#include "HomeScene.h"

USING_NS_CC;
using namespace ui;

Scene* SelectScene::CreateScene()
{
	return SelectScene::create();
}

bool SelectScene::init()
{
	if (!Scene::init())
	{
		return false;
	}


	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto bg = Sprite::create("/res/sprites/selectbg.jpg");
	bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, -1);
	//===============
	auto button = Button::create("/res/buttons/chose.png");
	button->setPosition(Vec2(visibleSize.width / 4 + origin.x, visibleSize.height / 1.5 + origin.y));
	button->setScale(0.75);
	this->addChild(button,0);

	auto buttonnewgame = Button::create("/res/buttons/newgame .png"); 
	buttonnewgame->setPosition(Vec2(visibleSize.width / 4 + origin.x, visibleSize.height / 1.3 + origin.y));
	buttonnewgame->setScale(0.65);
	this->addChild(buttonnewgame,1);
	buttonnewgame->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			auto gotoNext = CallFunc::create([]() {
				Director::getInstance()->replaceScene(HomeScene::CreateScene());
			});
			runAction(gotoNext);
		}
	});

	auto btcont = Button::create("/res/buttons/continue.png");
	btcont->setPosition(Vec2(visibleSize.width / 4 + origin.x, visibleSize.height / 1.6 + origin.y));
	btcont->setScale(0.65);
	this->addChild(btcont, 1);
	//===================
	
	auto button2 = Button::create("/res/buttons/chose.png");
	button2->setPosition(Vec2(visibleSize.width / 4.35+button2->getContentSize().width + origin.x, visibleSize.height / 1.5 + origin.y));
	button2->setScale(0.75);
	this->addChild(button2);

	auto buttonnewgame2 = Button::create("/res/buttons/newgame .png");
	buttonnewgame2->setPosition(Vec2(visibleSize.width / 4.35 + button2->getContentSize().width + origin.x, visibleSize.height / 1.3 + origin.y));
	buttonnewgame2->setScale(0.65);
	this->addChild(buttonnewgame2, 1);

	auto btcont2 = Button::create("/res/buttons/continue.png");
	btcont2->setPosition(Vec2(visibleSize.width / 4.35 + button2->getContentSize().width + origin.x, visibleSize.height / 1.6 + origin.y));
	btcont2->setScale(0.65);
	this->addChild(btcont2, 1);


	//========================
	auto button3 = Button::create("/res/buttons/chose.png");
	button3->setPosition(Vec2(visibleSize.width / 5 + button2->getContentSize().width*2 + origin.x, visibleSize.height / 1.5 + origin.y));
	button3->setScale(0.75);
	this->addChild(button3);

	auto buttonnewgame3 = Button::create("/res/buttons/newgame .png");
	buttonnewgame3->setPosition(Vec2(visibleSize.width / 5 + button2->getContentSize().width * 2 + origin.x, visibleSize.height / 1.3 + origin.y));
	buttonnewgame3->setScale(0.65);
	this->addChild(buttonnewgame3, 1);

	auto btcont3 = Button::create("/res/buttons/continue.png");
	btcont3->setPosition(Vec2(visibleSize.width / 5 + button2->getContentSize().width * 2, visibleSize.height / 1.6 + origin.y));
	btcont3->setScale(0.65);
	this->addChild(btcont3, 1);
	//========================
	auto buttondelete = Button::create("/res/buttons/delete.png");
	buttondelete->setPosition(Vec2(visibleSize.width / 4+ origin.x, visibleSize.height / 2.75 + origin.y));
	buttondelete->setScale(0.75);
	this->addChild(buttondelete);

	auto buttondelete2 = Button::create("/res/buttons/delete.png");
	buttondelete2->setPosition(Vec2(visibleSize.width / 4.35 + button2->getContentSize().width + origin.x, visibleSize.height / 2.75 + origin.y));
	buttondelete2->setScale(0.75);
	this->addChild(buttondelete2);
	
	auto buttondelete3 = Button::create("/res/buttons/delete.png");
	buttondelete3->setPosition(Vec2(visibleSize.width / 5 + button2->getContentSize().width*2 + origin.x, visibleSize.height / 2.75 + origin.y));
	buttondelete3->setScale(0.75);
	this->addChild(buttondelete3);
	//=============
	auto text = Sprite::create("/res/sprites/h.png");
	text->setAnchorPoint(Vec2(0, 0));
	text->setPosition(Vec2(visibleSize.width / 8 + origin.x, visibleSize.height / 30+ origin.y));
	text->setScaleX(1.5);
	text->setScaleY(0.4);
	this->addChild(text);
	return true;
}

