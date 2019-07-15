#include "Level1Scene.h"
#include "MainCharacter.h"
#include "ResourceManager.h"
using namespace std;

USING_NS_CC;

Scene* Level1Scene::CreateScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = Level1Scene::create();

	scene->addChild(layer);

	return scene;
}
bool Level1Scene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	MainCharacter::GetInstance()->Refresh();

	tileMap = ResourceManager::GetInstance()->GetTileMapById(3);
	upperTileMap = ResourceManager::GetInstance()->GetTileMapById(3);

	CreatePhysicsWorld("ob", "Player", this);

	CreateAllButton(this);

	AddListener();

	scheduleUpdate();

	return true;
}

void Level1Scene::update(float deltaTime)
{
	UpdateController();

	MainCharacter::GetInstance()->Update(deltaTime);

}

void Level1Scene::AddListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Level1Scene::OnTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Level1Scene::OnTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Level1Scene::OnTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	m_buttons[0]->addTouchEventListener(CC_CALLBACK_2(Level1Scene::SpecialAttack, this));
	m_buttons[1]->addTouchEventListener(CC_CALLBACK_2(Level1Scene::Evade, this));
	m_buttons[2]->addTouchEventListener(CC_CALLBACK_2(Level1Scene::NormalAttack, this));
	m_buttons[3]->addTouchEventListener(CC_CALLBACK_2(Level1Scene::Defend, this));
}

bool Level1Scene::OnTouchBegan(Touch* touch, Event* event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::MOVED;
	mCurrentTouchPoint = touch->getLocation();
	auto distance = camera->getPosition() - Director::getInstance()->getVisibleSize() / 2;
	mNextTouchPoint.x = mCurrentTouchPoint.x + distance.x;
	mNextTouchPoint.y = mCurrentTouchPoint.y + distance.y;
	return true;
}

bool Level1Scene::OnTouchEnded(Touch* touch, Event* event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::ENDED;
	mCurrentTouchPoint = Point(-1, -1);
	return true;
}

void Level1Scene::OnTouchMoved(Touch* touch, Event* event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::MOVED;
	mCurrentTouchPoint = touch->getLocation();
	auto distance = camera->getPosition() - Director::getInstance()->getVisibleSize() / 2;
	mNextTouchPoint.x = mCurrentTouchPoint.x + distance.x;
	mNextTouchPoint.y = mCurrentTouchPoint.y + distance.y;
}

void Level1Scene::NormalAttack(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->NormalAttack();

	}
}

void Level1Scene::SpecialAttack(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->SpecialAttack();
	}
}

void Level1Scene::Evade(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->Evade();
	}
}

void Level1Scene::Defend(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->Defend();
	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		MainCharacter::GetInstance()->StopDefend();
	}
}


//void Level1Scene::CreateMap()
//{
//	//tile map
//	mTileMap = ResourceManager::GetInstance()->GetTileMapById(3);//TMXTiledMap::create("res/tiledMaps/map1/map_lv1.tmx");
//	mTileMap->removeFromParent();
//	mTileMap->setAnchorPoint(Vec2(0,0));
//	mTileMap->setPosition(Point(0, 0));
//	addChild(mTileMap,0,99);
//	meta = mTileMap->getLayer("ob");
//	meta->setVisible(false);
//	collectMap = mTileMap->getLayer("collectItem");
//
//}
//
//void Level1Scene::update(float deltaTime)
//{
//}
//
//void Level1Scene::setViewPointCenter(cocos2d::Point position)
//{
//	auto winSize = Director::getInstance()->getWinSize();
//
//	int x = MAX(position.x, winSize.width / 2);
//	int y = MAX(position.y, winSize.height / 2);
//	x = MIN(x, (mTileMap->getMapSize().width * this->mTileMap->getTileSize().width) - winSize.width / 2);
//	y = MIN(y, (mTileMap->getMapSize().height * mTileMap->getTileSize().height) - winSize.height / 2);
//	auto actualPosition = Point(x, y);
//
//	auto centerOfView = Point(winSize.width / 2, winSize.height / 2);
//	auto viewPoint = centerOfView - actualPosition;
//	this->setPosition(viewPoint);
//}
//
//void Level1Scene::setPlayerPosition(cocos2d::Point position)
//{
//	log("move->");
//	Point tileCoord = this->tileCoordForPosition(position);
//	int tileGid = meta->getTileGIDAt(tileCoord);
//	if (tileGid)
//	{
//		auto properties = mTileMap->getPropertiesForGID(tileGid).asValueMap();
//		if (properties.size()>0)
//		{
//			auto collision = properties["Collidable"].asString();
//			if ("True"==collision)
//			{
//				log("opps!\n");
//				return;
//			}
//		}
//	}
//	tileGid = collectMap->getTileGIDAt(tileCoord);
//	if (tileGid)
//	{
//		auto properties = mTileMap->getPropertiesForGID(tileGid).asValueMap();
//		if (properties.size()>0)
//		{
//			auto collision = properties["Collectable"].asString();
//			if ("True" == collision)
//			{
//				log("collected");
//				collectMap->removeTileAt(tileCoord);
//				return;
//			}
//		}
//	}
//	player->setPosition(position);
//}
//
//cocos2d::Point Level1Scene::tileCoordForPosition(cocos2d::Point position)
//{
//	int x = position.x / mTileMap->getTileSize().width;
//	int y = ((mTileMap->getMapSize().height*mTileMap->getTileSize().height) - position.y)
//			/ mTileMap->getTileSize().height;
//	return cocos2d::Point(x,y);
//}
//
//void Level1Scene::onTouchEnded(Touch *touch, Event *unused_event)
//{
//	
//	auto touchLocation = touch->getLocationInView();
//	touchLocation = Director::getInstance()->convertToGL(touchLocation);
//	touchLocation = this->convertToNodeSpace(touchLocation);
//	
//	auto playerPos = player->getPosition();
//	auto diff = touchLocation - playerPos;
//	if (abs(diff.x) > abs(diff.y)) {
//		if (diff.x > 0) {
//			playerPos.x += mTileMap->getTileSize().width / 2;
//		}
//		else {
//			playerPos.x -= mTileMap->getTileSize().width / 2;
//		}
//	}
//	else {
//		if (diff.y > 0) {
//			playerPos.y += mTileMap->getTileSize().height / 2;
//		}
//		else {
//			playerPos.y -= mTileMap->getTileSize().height / 2;
//		}
//	}
//
//	if (playerPos.x <= (mTileMap->getMapSize().width * mTileMap->getMapSize().width) &&
//		playerPos.y <= (mTileMap->getMapSize().height * mTileMap->getMapSize().height) &&
//		playerPos.y >= 0 &&
//		playerPos.x >= 0)
//	{
//		this->setPlayerPosition(playerPos);
//	}
//	this->setViewPointCenter(player->getPosition());
//	log("clicked!");
//}
