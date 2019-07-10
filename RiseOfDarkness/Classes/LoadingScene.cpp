
//#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
#include "LoadingScene.h"
#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* LoadingScene::CreateScene()
{
	auto scene = Scene::create();
	auto layer = LoadingScene::create();
	scene->addChild(layer);
	return scene;
}

bool LoadingScene::init()
{

	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("/res/sprites/bg.png");
	bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, -1);

	auto label = Label::createWithTTF("Loading...", "fonts/Marker Felt.ttf", 30);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2.6 - label->getContentSize().height));
	label->enableOutline(Color4B::RED, 1);
	this->addChild(label, 1);
	

	auto loadingBarBG = Sprite::create("loadingbar_bg.png");
	loadingBarBG->setPosition(visibleSize.width / 2, visibleSize.height / 4);
	addChild(loadingBarBG);

	auto loadingBar = ui::LoadingBar::create("loadingbar.png");
	loadingBar->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4));
	loadingBar->setDirection(ui::LoadingBar::Direction::LEFT);
	loadingBar->setPercent(0);
	this->addChild(loadingBar);
	this->schedule([=](float delta) {
		float percent = loadingBar->getPercent();
		percent++;
		loadingBar->setPercent(percent);
		if (percent >= 100.0f)
		{
			this->unschedule("updateLoadingBar");
		}

	}, 0.029f, "updateLoadingBar");
	auto gotoNext = CallFunc::create([]() {
		Director::getInstance()->replaceScene(MainMenuScene::create());
	});
	auto sequence = Sequence::create(DelayTime::create(3), gotoNext, nullptr);
	runAction(sequence);

	return true;
}

