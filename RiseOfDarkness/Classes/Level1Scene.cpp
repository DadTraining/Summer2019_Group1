#include "Level1Scene.h"
#include "MainCharacter.h"
#include "ResourceManager.h"
#include "SpearMoblin.h"
#include "BowMoblin.h"
#include "MapScene.h"
#include "HomeScene.h"

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
	if (!MainCharacter::GetInstance()->IsAlive())
	{
		m_buttons[7]->setVisible(true);
		m_buttons[8]->setVisible(true);
		gameover->setVisible(true);
	}
}

void Level1Scene::CreateMonster()
{
	float x1, y1;
	int direction1;
	auto spearGoblinGroup = tileMap->getObjectGroup("spearMoblin");
	int amount1 = 4;
	char str1[10];
	for (int i = 1; i <= amount1; i++)
	{
		sprintf(str1, "%02d", i);
		direction1 = spearGoblinGroup->getObject(str1)["direction"].asInt();
		x1 = spearGoblinGroup->getObject(str1)["x"].asFloat();
		y1 = spearGoblinGroup->getObject(str1)["y"].asFloat();
		SpearMoblin *spearMoblin = new SpearMoblin(this, direction1, Vec2(x1, y1));
		spearMoblin->GetPhysicsBody()->setGroup(i - 1);
		m_SpearMoblins.push_back(spearMoblin);
	}

	float x2, y2;
	int direction2;
	auto bowGoblinGroup = tileMap->getObjectGroup("bowMoblin");
	int amount2 = 3;
	char str2[10];
	for (int i = 1; i <= amount2; i++)
	{
		sprintf(str2, "%02d", i);
		direction2 = bowGoblinGroup->getObject(str2)["direction"].asInt();
		x2 = bowGoblinGroup->getObject(str2)["x"].asFloat();
		y2 = bowGoblinGroup->getObject(str2)["y"].asFloat();
		BowMoblin *bowMoblin = new BowMoblin(this, direction2, Vec2(x2, y2));
		bowMoblin->GetPhysicsBody()->setGroup(i - 1 + amount1);
		m_SpearMoblins.push_back(bowMoblin);
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

	m_buttons[4]->addClickEventListener([&](Ref* event) {
		if (!m_buttons[5]->isVisible())
		{
			m_buttons[5]->setVisible(true);
			m_buttons[6]->setVisible(true);
			Director::getInstance()->pause();
		}
	});

	m_buttons[5]->addClickEventListener([&](Ref* event) {
		m_buttons[5]->setVisible(false);
		m_buttons[6]->setVisible(false);
		Director::getInstance()->resume();
	});

	m_buttons[6]->addClickEventListener([&](Ref* event) {
		m_buttons[5]->setVisible(false);
		m_buttons[6]->setVisible(false);
		Director::getInstance()->resume();
		m_SpearMoblins.clear();

		auto gotoMap = CallFunc::create([] {
			Director::getInstance()->replaceScene(MapScene::CreateScene());
		});
		runAction(gotoMap);
	});

	m_buttons[7]->addClickEventListener([&](Ref* event) {
		m_SpearMoblins.clear();
		auto gotoMap = CallFunc::create([] {
			Director::getInstance()->replaceScene(HomeScene::CreateScene());
		});
		runAction(gotoMap);
	});

	m_buttons[8]->addClickEventListener([&](Ref* event) {
		m_SpearMoblins.clear();
		auto gotoMap = CallFunc::create([] {
			Director::getInstance()->replaceScene(Level1Scene::CreateScene());
		});
		runAction(gotoMap);
	});

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

	// ARROW COLLIDE WITH OBSTACLE
	if ((a->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK && b->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK && b->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK))
	{
		if (a->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK)
		{
			MainCharacter::GetInstance()->GetListArrow()[a->getGroup()]->SetVisible(false);
		}
		else if (b->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK)
		{
			MainCharacter::GetInstance()->GetListArrow()[b->getGroup()]->SetVisible(false);
		}
	}
	
	// MAIN CHARACTER SLASH SPEARMOBLIN
	if ((a->getCollisionBitmask() == MainCharacter::SLASH_BITMASK && b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK && b->getCollisionBitmask() == MainCharacter::SLASH_BITMASK))
	{
		if (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		{
			m_SpearMoblins[a->getGroup()]->GetDamage(MainCharacter::GetInstance()->GetAttack());
		}
		else if (b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		{
			m_SpearMoblins[b->getGroup()]->GetDamage(MainCharacter::GetInstance()->GetAttack());
		}
	}
	
	// MAIN CHARACTER'S ARROW COLLIDE SPEARMOBLIN
	if ((a->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK && b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK && b->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK))
	{
		if (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		{
			m_SpearMoblins[a->getGroup()]->GetDamage(MainCharacter::NORMAL_ARROW);
			MainCharacter::GetInstance()->GetListArrow()[b->getGroup()]->SetVisible(false);
		}
		else if (b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		{
			m_SpearMoblins[b->getGroup()]->GetDamage(MainCharacter::NORMAL_ARROW);
			MainCharacter::GetInstance()->GetListArrow()[a->getGroup()]->SetVisible(false);
		}
	}

	// SPEARMOBLIN PIERCE MAIN CHARACTER
	if ((a->getCollisionBitmask() == MainCharacter::PIERCE_BITMASK && b->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK && b->getCollisionBitmask() == MainCharacter::PIERCE_BITMASK))
	{
		MainCharacter::GetInstance()->GetDamage(MainCharacter::SPEARMOBLIN_DAMAGE);
	}

	// BOWMOBLIN ARROW DAMAGE MAIN CHARACTER
	if ((a->getCollisionBitmask() == MainCharacter::BOWMOBLIN_ARROW_BITMASK && b->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK && b->getCollisionBitmask() == MainCharacter::BOWMOBLIN_ARROW_BITMASK))
	{
		MainCharacter::GetInstance()->GetDamage(MainCharacter::BOWMOBLIN_DAMAGE);
		
	}

	// SPEARMOBLIN COLLIDE OBSTACLES
	if ((a->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK && b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK && b->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK))
	{
		if (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		{
			m_SpearMoblins[a->getGroup()]->SetPreventRun();
			m_SpearMoblins[a->getGroup()]->ReverseDirection();
		}
		else if (b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		{
			m_SpearMoblins[b->getGroup()]->SetPreventRun();
			m_SpearMoblins[b->getGroup()]->ReverseDirection();
		}
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