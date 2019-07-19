#include "InputNameScene.h"
#include "NotifyScene.h"
#include <String.h>
#include <iostream>
#include "HomeScene.h"
#include "Level2Scene.h"
#include "ResourceManager.h"
#include "MainCharacter.h"
#include "Level1Scene.h"
USING_NS_CC;

Scene* NotifyScene::CreateScene()
{
	auto scene = Scene::create();
	auto layer = NotifyScene::create();
	scene->addChild(layer);
	return scene;
}

bool NotifyScene::init()
{

	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = ResourceManager::GetInstance()->GetSpriteById(7);//Sprite::create("/res/sprites/selectbg.jpg");
	bg->removeFromParent();
	bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, -1);

	auto text = Sprite::create("res/sprites/inputName/note.png");
	//text->removeFromParent();
	text->setScaleY(1.85);
	text->setScaleX(1.9);
	text->setAnchorPoint(Vec2(0, 0));
	text->setPosition(Vec2(visibleSize.width / 4 + origin.x+20, visibleSize.height / 6 + origin.y));
	this->addChild(text);

	auto label = ResourceManager::GetInstance()->GetLabelById(0);//Label::createWithTTF("Enter your name ", "fonts/Marker Felt.ttf", 40);
	label->removeFromParent();
	label->setString("empty name, input your name please!");
	label->setColor(Color3B::WHITE);
	label->setBMFontSize(16);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.5 - 100));
	//label->enableOutline(Color4B::RED, 1);
	this->addChild(label, 1);


	auto buttonexit = ui::Button::create("/res/buttons/notify/exit.png","/res/buttons/notify/exit_bg.png");
	//buttonfinish->removeFromParent();
	buttonexit->setPosition(Vec2(origin.x + visibleSize.width / 2+185,
		origin.y + visibleSize.height / 1.5 +7));
	buttonexit->setScale(0.2);

	buttonexit->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		
				auto gotoNext = CallFunc::create([]() {
					Director::getInstance()->replaceScene(InputNameScene::CreateScene());
				});
				runAction(gotoNext);
	});

	this->addChild(buttonexit);
	return true;
}
