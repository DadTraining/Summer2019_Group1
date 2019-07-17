#include "Level1Scene.h"
#include "MainCharacter.h"
#include "ResourceManager.h"
#include "SpearMoblin.h"
using namespace std;

USING_NS_CC;

Scene* Level1Scene::CreateScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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

	MainCharacter::GetInstance()->Refresh();

	tileMap = ResourceManager::GetInstance()->GetTileMapById(3);
	upperTileMap = ResourceManager::GetInstance()->GetTileMapById(4);

	CreatePhysicsWorld("obstacles", "mc", this);

	CreateAllButton(this);

	AddListener();

	CreateMonster();



	scheduleUpdate();

	return true;
}

void Level1Scene::update(float deltaTime)
{
	UpdateController();

	UpdateInfoBar();

	MainCharacter::GetInstance()->Update(deltaTime);

	SetCamera(mainCharacter->getPosition());

	for (int i = 0; i < m_SpearMoblins.size(); i++)
	{
		if (m_SpearMoblins[i]->GetSprite()->isVisible())
		{
			m_SpearMoblins[i]->Update(deltaTime);
		}
	}
}

void Level1Scene::CreateMonster()
{
	float x, y;
	auto spearGoblinGroup = tileMap->getObjectGroup("SpearMoblin");
	int amount = 3;
	char str[10];
	for (int i = 1; i <= amount; i++)
	{
		SpearMoblin *spearMoblin = new SpearMoblin(this, i + 2);
		sprintf(str, "%02d", i);
		x = spearGoblinGroup->getObject(str)["x"].asFloat();
		y = spearGoblinGroup->getObject(str)["y"].asFloat();
		spearMoblin->GetSprite()->setPosition(x, y);
		m_SpearMoblins.push_back(spearMoblin);
	}
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

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Level1Scene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
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

bool Level1Scene::onContactBegin(PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	// MAIN CHARACTER WITH OBSTACLES
	if ((a->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK && b->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK && b->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK))
	{
		if (MainCharacter::GetInstance()->GetCurrentState() == MainCharacter::ROLL_BACK || MainCharacter::GetInstance()->GetCurrentState() == MainCharacter::ROLL_LEFT ||
			MainCharacter::GetInstance()->GetCurrentState() == MainCharacter::ROLL_FRONT)
		{
			mainCharacter->stopAllActions();
		}
		if (MainCharacter::GetInstance()->GetDirection() == 1)
		{
			mainCharacter->setPositionY(mainCharacter->getPositionY() - MainCharacter::GetInstance()->GetSpeed());
			MainCharacter::GetInstance()->SetPreventRun(1);
		}
		else if (MainCharacter::GetInstance()->GetDirection() == 2)
		{
			mainCharacter->setPositionY(mainCharacter->getPositionY() + MainCharacter::GetInstance()->GetSpeed());
			MainCharacter::GetInstance()->SetPreventRun(2);
		}
		else if (MainCharacter::GetInstance()->GetDirection() == 3)
		{
			mainCharacter->setPositionX(mainCharacter->getPositionX() + MainCharacter::GetInstance()->GetSpeed());
			MainCharacter::GetInstance()->SetPreventRun(3);
		}
		else if (MainCharacter::GetInstance()->GetDirection() == 4)
		{
			mainCharacter->setPositionX(mainCharacter->getPositionX() - MainCharacter::GetInstance()->GetSpeed());
			MainCharacter::GetInstance()->SetPreventRun(4);
		}
	}

	// MAIN CHARACTER SLASH SPEARMOBLIN
	if ((a->getCollisionBitmask() == MainCharacter::SLASH_BITMASK && b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK && b->getCollisionBitmask() == MainCharacter::SLASH_BITMASK))
	{
		m_SpearMoblins.at(a->getGroup())->GetDamage(MainCharacter::GetInstance()->GetAttack());
	}

	// SPEARMOBLIN PIERCE MAIN CHARACTER
	if ((a->getCollisionBitmask() == MainCharacter::PIERCE_BITMASK && b->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK && b->getCollisionBitmask() == MainCharacter::PIERCE_BITMASK))
	{
		MainCharacter::GetInstance()->GetDamage(MainCharacter::SPEARMOBLIN_DAMAGE);
	}
	return true;
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