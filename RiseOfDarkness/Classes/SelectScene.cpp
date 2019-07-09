#include "SelectScene.h"
#include "ui/CocosGUI.h"
#include "PlayScene.h"


using namespace std;
USING_NS_CC;

int lv = 1;
vector<ui::Button*> buttonList;
Scene* SelectScene::CreateScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = SelectScene::create();

	scene->addChild(layer);

	return scene;
}
bool SelectScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto btnMap = TMXTiledMap::create("res/tiledMaps/SelectScene/selectMap.tmx");
	btnMap->setAnchorPoint(Point(0, 0));
	btnMap->setPosition(0,0);
	btnMap->setScale(visibleSize.width / (btnMap->getMapSize().width*btnMap->getTileSize().width),
		visibleSize.height / (btnMap->getMapSize().height*btnMap->getTileSize().height));
	addChild(btnMap, 0);
	auto background = Sprite::create("res/tiledMaps/SelectScene/background.png");
	background->setAnchorPoint(Vec2(0.5, 0.5));
	background->setPosition(Point(visibleSize/2));
	
	TMXObjectGroup *btnGroup = btnMap->objectGroupNamed("ObjButton");

	for (int i = 0; i < 11; i++)
	{
		auto btnPos = btnGroup->getObject("btnLv"+to_string(i+1));
		int x = btnPos["x"].asInt();
		int y = btnPos["y"].asInt();
		auto btn = ui::Button::create("res/tiledMaps/SelectScene/x.png", "res/tiledMaps/SelectScene/x_disabled.png");
		btn->setPosition(Point(x, y));
		btn->setScale(0.5);
		btn->setVisible(false);
		addChild(btn, 1);
		btn->addClickEventListener(CC_CALLBACK_1(SelectScene::LoadLevel, this, i+1));
		buttonList.push_back(btn);
	}

	for (int i = 0; i < lv; i++)
	{
		if (i>11)
		{
			break;
		}
		buttonList[i]->setVisible(true);
	}

	return true;
}

void SelectScene::LoadLevel(Ref *pSender,int level)
{
	switch (level)
	{
	case 1:
		Director::getInstance()->replaceScene(PlayScene::CreateScene());
	default:
		break;
	}
}

void SelectScene::CreateMap()
{

}

void SelectScene::onTouchEnded(Touch *touch, Event *unused_event)
{
}
