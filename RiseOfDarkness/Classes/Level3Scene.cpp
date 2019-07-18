#include "Level3Scene.h"
#include "MainCharacter.h"
#include "ResourceManager.h"
#include "cocos2d.h"
using namespace std;

USING_NS_CC;

Scene* Level3Scene::CreateScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = Level3Scene::create();

	scene->addChild(layer);

	return scene;
}
bool Level3Scene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	MainCharacter::GetInstance()->Refresh();

	tileMap = TMXTiledMap::create("res/tiledMaps/map3/lv3.tmx");
	upperTileMap = TMXTiledMap::create("res/tiledMaps/map3/upperLv3.tmx");

	
	CreatePhysicsWorld("ob", "mc", this);

	CreateAllButton(this);

	AddListener();


	scheduleUpdate();


	

	return true;
}

void Level3Scene::update(float deltaTime)
{
	UpdateController();

	UpdateInfoBar();

	MainCharacter::GetInstance()->Update(deltaTime);

	SetCamera(mainCharacter->getPosition());
}

void Level3Scene::AddListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Level3Scene::OnTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Level3Scene::OnTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Level3Scene::OnTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	m_buttons[0]->addTouchEventListener(CC_CALLBACK_2(Level3Scene::SpecialAttack, this));
	m_buttons[1]->addTouchEventListener(CC_CALLBACK_2(Level3Scene::Evade, this));
	m_buttons[2]->addTouchEventListener(CC_CALLBACK_2(Level3Scene::NormalAttack, this));
	m_buttons[3]->addTouchEventListener(CC_CALLBACK_2(Level3Scene::Defend, this));

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Level3Scene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool Level3Scene::OnTouchBegan(Touch* touch, Event* event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::MOVED;
	mCurrentTouchPoint = touch->getLocation();
	auto distance = camera->getPosition() - Director::getInstance()->getVisibleSize() / 2;
	mNextTouchPoint.x = mCurrentTouchPoint.x + distance.x;
	mNextTouchPoint.y = mCurrentTouchPoint.y + distance.y;
	return true;
}

bool Level3Scene::OnTouchEnded(Touch* touch, Event* event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::ENDED;
	mCurrentTouchPoint = Point(-1, -1);
	return true;
}

void Level3Scene::OnTouchMoved(Touch* touch, Event* event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::MOVED;
	mCurrentTouchPoint = touch->getLocation();
	auto distance = camera->getPosition() - Director::getInstance()->getVisibleSize() / 2;
	mNextTouchPoint.x = mCurrentTouchPoint.x + distance.x;
	mNextTouchPoint.y = mCurrentTouchPoint.y + distance.y;
}

bool Level3Scene::onContactBegin(PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	if ((a->getCollisionBitmask() == MainCharacter::mainCharacterBitMask && b->getCollisionBitmask() == MainCharacter::obstacleBitMask)
		|| (a->getCollisionBitmask() == MainCharacter::obstacleBitMask && b->getCollisionBitmask() == MainCharacter::mainCharacterBitMask))
	{
		if (MainCharacter::GetInstance()->GetDirection() == 1)
		{
			mainCharacter->setPositionY(mainCharacter->getPositionY() - 15);
		}
		else if (MainCharacter::GetInstance()->GetDirection() == 2)
		{
			mainCharacter->setPositionY(mainCharacter->getPositionY() + 15);
		}
		else if (MainCharacter::GetInstance()->GetDirection() == 3)
		{
			mainCharacter->setPositionX(mainCharacter->getPositionX() + 15);
		}
		else if (MainCharacter::GetInstance()->GetDirection() == 4)
		{
			mainCharacter->setPositionX(mainCharacter->getPositionX() - 15);
		}
	}
	return true;
}

void Level3Scene::NormalAttack(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->NormalAttack();

	}
}

void Level3Scene::SpecialAttack(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->SpecialAttack();
	}
}

void Level3Scene::Evade(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->Evade();
	}
}

void Level3Scene::Defend(Ref* sender, ui::Widget::TouchEventType type)
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