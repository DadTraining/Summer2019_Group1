#include "InputNameScene.h"
#include "UI/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include <String.h>
#include <iostream>
USING_NS_CC;

Scene* InputNameScene::CreateScene()
{
	auto scene = Scene::create();
	auto layer = InputNameScene::create();
	scene->addChild(layer);
	return scene;
}

bool InputNameScene::init()
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

	auto text = Sprite::create("/res/sprites/inputname.png");
	text->setAnchorPoint(Vec2(0, 0));
	text->setPosition(Vec2(visibleSize.width / 4 + origin.x, visibleSize.height / 6 + origin.y));
	this->addChild(text);

	auto label = Label::createWithTTF("Enter your name ", "fonts/Marker Felt.ttf", 40);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 1.5-10));
	label->enableOutline(Color4B::RED, 1);
	this->addChild(label, 1);
	
	auto textField = ui::TextField::create("enter here.. ", "fonts/Marker Felt.ttf", 30);
	
	textField->setPasswordEnabled(false);
	textField->setMaxLength(50);
	textField->setPosition(Vec2(visibleSize.width / 2, visibleSize.height/1.5-70));
	textField->setColor(Color3B(112, 40, 4));
	/*textField->addClickEventListener([](Ref* event) {
		log("TextField: %s", textField.getString().c_str);
	});*/
	
    textField->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		std::cout << "editing a TextField" << std::endl;
	});
	
	this->addChild(textField,2);

	auto buttonfinish = ui::Button::create("/res/buttons/finish.png");
	buttonfinish->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 3 + origin.y));
	//buttonfinish->setScaleX(2);

	this->addChild(buttonfinish);
	return true;
}
