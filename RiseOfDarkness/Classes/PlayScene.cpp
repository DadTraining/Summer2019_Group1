#include "PlayScene.h"

using namespace std;
USING_NS_CC;
Size visibleSize;

Scene* PlayScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = PlayScene::create();

	scene->addChild(layer);

	return scene;
}
bool PlayScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	CreateMap();
	
	//AddListener();

	player = Sprite::create("res/sprites/link-5_65.png");
	TMXObjectGroup *playerGroup = mTileMap->objectGroupNamed("Player");
	auto spawnPoint = playerGroup->getObject("mc");
	int x = spawnPoint["x"].asInt();
	int y = spawnPoint["y"].asInt();
	//log("%d, %d", x, y);
	player->setPosition(Vec2(x + mTileMap->getTileSize().width/2,y + mTileMap->getTileSize().height/2));
	player->setScale(1.7f);
	player->setAnchorPoint(Vec2(0.5, 0));
	auto mc_Body = PhysicsBody::createBox(player->getContentSize(), PHYSICSSHAPE_MATERIAL_DEFAULT);
	player->setPhysicsBody(mc_Body);
	addChild(player, 0);
	scheduleUpdate();
	setViewPointCenter(player->getPosition());
	
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool {return true;};
	listener->onTouchEnded = CC_CALLBACK_2(PlayScene::onTouchEnded, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->setTouchEnabled(true);
	return true;
}

void PlayScene::CreateMap()
{
	//tile map
	mTileMap = TMXTiledMap::create("res/tiledMaps/map1/map_lv1.tmx");
	mTileMap->setAnchorPoint(Vec2(0,0));
	mTileMap->setPosition(Point(0, 0));
	addChild(mTileMap,0,99);
	meta = mTileMap->getLayer("ob");
	meta->setVisible(false);
	collectMap = mTileMap->getLayer("collectItem");

}

void PlayScene::update(float deltaTime)
{
}

void PlayScene::initObstacles()
{
}

void PlayScene::setViewPointCenter(cocos2d::Point position)
{
	auto winSize = Director::getInstance()->getWinSize();

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (mTileMap->getMapSize().width * this->mTileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (mTileMap->getMapSize().height * mTileMap->getTileSize().height) - winSize.height / 2);
	auto actualPosition = Point(x, y);

	auto centerOfView = Point(winSize.width / 2, winSize.height / 2);
	auto viewPoint = centerOfView - actualPosition;
	this->setPosition(viewPoint);
}

void PlayScene::setPlayerPosition(cocos2d::Point position)
{
	log("move->");
	Point tileCoord = this->tileCoordForPosition(position);
	int tileGid = meta->getTileGIDAt(tileCoord);
	if (tileGid)
	{
		auto properties = mTileMap->getPropertiesForGID(tileGid).asValueMap();
		if (properties.size()>0)
		{
			auto collision = properties["Collidable"].asString();
			if ("True"==collision)
			{
				log("opps!\n");
				return;
			}
		}
	}
	tileGid = collectMap->getTileGIDAt(tileCoord);
	if (tileGid)
	{
		auto properties = mTileMap->getPropertiesForGID(tileGid).asValueMap();
		if (properties.size()>0)
		{
			auto collision = properties["Collectable"].asString();
			if ("True" == collision)
			{
				log("collected");
				collectMap->removeTileAt(tileCoord);
				return;
			}
		}
	}
	player->setPosition(position);
}

cocos2d::Point PlayScene::tileCoordForPosition(cocos2d::Point position)
{
	int x = position.x / mTileMap->getTileSize().width;
	int y = ((mTileMap->getMapSize().height*mTileMap->getTileSize().height) - position.y)
			/ mTileMap->getTileSize().height;
	return cocos2d::Point(x,y);
}

void PlayScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	
	auto touchLocation = touch->getLocationInView();
	touchLocation = Director::getInstance()->convertToGL(touchLocation);
	touchLocation = this->convertToNodeSpace(touchLocation);
	
	auto playerPos = player->getPosition();
	auto diff = touchLocation - playerPos;
	if (abs(diff.x) > abs(diff.y)) {
		if (diff.x > 0) {
			playerPos.x += mTileMap->getTileSize().width / 2;
		}
		else {
			playerPos.x -= mTileMap->getTileSize().width / 2;
		}
	}
	else {
		if (diff.y > 0) {
			playerPos.y += mTileMap->getTileSize().height / 2;
		}
		else {
			playerPos.y -= mTileMap->getTileSize().height / 2;
		}
	}

	if (playerPos.x <= (mTileMap->getMapSize().width * mTileMap->getMapSize().width) &&
		playerPos.y <= (mTileMap->getMapSize().height * mTileMap->getMapSize().height) &&
		playerPos.y >= 0 &&
		playerPos.x >= 0)
	{
		this->setPlayerPosition(playerPos);
	}
	this->setViewPointCenter(player->getPosition());
	log("clicked!");
}
