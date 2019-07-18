#include "Level2Scene.h"
#include "ResourceManager.h"
#include "MainCharacter.h"
#include "MapScene.h"
#include <math.h>

USING_NS_CC;
using namespace std;

Scene* Level2Scene::CreateScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	auto layer = Level2Scene::create();

	scene->addChild(layer);

	return scene;
}

bool Level2Scene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	MainCharacter::GetInstance()->Refresh();

	tileMap = ResourceManager::GetInstance()->GetTileMapById(4);
	upperTileMap = ResourceManager::GetInstance()->GetTileMapById(5);

	CreatePhysicsWorld("obstacles", "mc", this);

	for (int i = 0; i < 3; i++)
	{
		auto mt = new Monster(this);
		m_monster.push_back(mt);
	}

	for (int j = 0; j < 3; j++)
	{
		string nameObj = "mt" + to_string(j + 1);
		listMonster[j] = m_monster[j]->GetSprite();
		auto obj = tileMap->objectGroupNamed("mt");
		float x = obj->getObject(nameObj)["x"].asFloat();
		float y = obj->getObject(nameObj)["y"].asFloat();
		listMonster[j]->setPosition(x, y);
		m_monster[j]->Run();
	}

	CreateAllButton(this);

	AddListener();

	scheduleUpdate();

	return true;
}

void Level2Scene::update(float deltaTime)
{
	UpdateController();
	UpdateInfoBar();
	m_maincharacter->GetInstance()->Update(deltaTime);
	SetCamera(mainCharacter->getPosition());
	for (int j = 0; j < 3; j++)
	{
		if (m_monster[j]->Detect(mainCharacter->getPosition()) == true)
		{
			m_monster[j]->StopRun();
			m_monster[j]->Hit();
		}
		else
		{
			m_monster[j]->StartRun();
		}
	}
}

void Level2Scene::AddListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Level2Scene::OnTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Level2Scene::OnTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Level2Scene::OnTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	m_buttons[0]->addTouchEventListener(CC_CALLBACK_2(Level2Scene::SpecialAttack, this));
	m_buttons[1]->addTouchEventListener(CC_CALLBACK_2(Level2Scene::Evade, this));
	m_buttons[2]->addTouchEventListener(CC_CALLBACK_2(Level2Scene::NormalAttack, this));
	m_buttons[3]->addTouchEventListener(CC_CALLBACK_2(Level2Scene::Defend, this));

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Level2Scene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool Level2Scene::OnTouchBegan(Touch* touch, Event* event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::MOVED;
	mCurrentTouchPoint = touch->getLocation();
	auto distance = camera->getPosition() - Director::getInstance()->getVisibleSize() / 2;
	mNextTouchPoint.x = mCurrentTouchPoint.x + distance.x;
	mNextTouchPoint.y = mCurrentTouchPoint.y + distance.y;
	return true;
}

bool Level2Scene::OnTouchEnded(Touch* touch, Event* event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::ENDED;
	mCurrentTouchPoint = Point(-1, -1);
	return true;
}

void Level2Scene::OnTouchMoved(Touch* touch, Event* event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::MOVED;
	mCurrentTouchPoint = touch->getLocation();
	auto distance = camera->getPosition() - Director::getInstance()->getVisibleSize() / 2;
	mNextTouchPoint.x = mCurrentTouchPoint.x + distance.x;
	mNextTouchPoint.y = mCurrentTouchPoint.y + distance.y;
}

void Level2Scene::NormalAttack(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		m_maincharacter->GetInstance()->SpecialAttack();
	}
}

void Level2Scene::SpecialAttack(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		m_maincharacter->GetInstance()->NormalAttack();
	}
}

void Level2Scene::Evade(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		m_maincharacter->GetInstance()->Evade();
	}
}

void Level2Scene::Defend(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		m_maincharacter->GetInstance()->Defend();
	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		m_maincharacter->GetInstance()->StopDefend();
	}
}

bool Level2Scene::onContactBegin(PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	if ((a->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK && b->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK && b->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK) 
		|| (a->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK && b->getCollisionBitmask() == Monster::MONSTER_BITMASK)
		|| (a->getCollisionBitmask() == Monster::MONSTER_BITMASK && b->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK))
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



