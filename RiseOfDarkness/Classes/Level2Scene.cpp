#include "Level2Scene.h"
#include "MainCharacter.h"
#include "ResourceManager.h"
#include "MapScene.h"
#include "HomeScene.h"
#include "Monster.h"

using namespace std;

USING_NS_CC;

Scene* Level2Scene::CreateScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
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

	tileMap = ResourceManager::GetInstance()->GetTileMapById(5);
	upperTileMap = ResourceManager::GetInstance()->GetTileMapById(6);

	CreatePhysicsWorld("obstacles", "mc", this);

	CreateAllButton(this);

	AddListener();

	CreateMonster();



	scheduleUpdate();

	return true;
}

void Level2Scene::update(float deltaTime)
{
	UpdateController();

	UpdateInfoBar();

	MainCharacter::GetInstance()->Update(deltaTime);

	SetCamera(mainCharacter->getPosition());

	for (int i = 0; i < m_enemies.size(); i++)
	{
		if (m_enemies[i]->GetSprite()->isVisible())
		{
			m_enemies[i]->Update(deltaTime);
		}
	}

	if (!MainCharacter::GetInstance()->IsAlive())
	{
		m_buttons[7]->setVisible(true);
		m_buttons[8]->setVisible(true);
		gameover->setVisible(true);
	}
	gold->setString(std::to_string(MainCharacter::GetInstance()->GetGold()));
}

void Level2Scene::CreateMonster()
{
	float x2, y2;
	int direction2;
	auto rope = tileMap->getObjectGroup("rope");
	int amount2 = 4;
	char str2[10];
	for (int i = 1; i <= amount2; i++)
	{
		sprintf(str2, "%02d", i);
		direction2 = rope->getObject(str2)["direction"].asInt();
		x2 = rope->getObject(str2)["x"].asFloat();
		y2 = rope->getObject(str2)["y"].asFloat();
		Monster *monster = new Monster(this, direction2, Vec2(x2, y2), i - 1);
		monster->GetPhysicsBody()->setGroup(i - 1);
		m_enemies.push_back(monster);
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

	m_buttons[4]->addClickEventListener([&](Ref* event) {
		if (!m_buttons[5]->isVisible())
		{
			m_buttons[4]->setVisible(false);
			m_buttons[5]->setVisible(true);
			m_buttons[6]->setVisible(true);
			m_buttons[7]->setVisible(true);
			Director::getInstance()->pause();
		}
	});

	m_buttons[5]->addClickEventListener([&](Ref* event) {
		m_buttons[4]->setVisible(true);
		m_buttons[5]->setVisible(false);
		m_buttons[6]->setVisible(false);
		m_buttons[7]->setVisible(false);
		Director::getInstance()->resume();
	});

	m_buttons[6]->addClickEventListener([&](Ref* event) {
		Director::getInstance()->resume();
		auto gotoMap = CallFunc::create([] {
			Director::getInstance()->replaceScene(HomeScene::CreateScene());
		});
		runAction(gotoMap);
	});

	m_buttons[7]->addClickEventListener([&](Ref* event) {
		Director::getInstance()->resume();
		auto gotoMap = CallFunc::create([] {
			Director::getInstance()->replaceScene(Level2Scene::CreateScene());
		});
		runAction(gotoMap);
	});

	m_buttons[9]->addClickEventListener([&](Ref* event) {
		MainCharacter::GetInstance()->GetInventory()->RemoveItem(0, 0);
	});

	m_buttons[10]->addClickEventListener([&](Ref* event) {
		MainCharacter::GetInstance()->GetInventory()->RemoveItem(1, 1);
	});

	m_buttons[11]->addClickEventListener(CC_CALLBACK_1(Level2Scene::OpenInventory, this));

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

bool Level2Scene::onContactBegin(PhysicsContact& contact)
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


	// MAIN CHARACTER SLASH ROPE MONSTER
	if ((a->getCollisionBitmask() == MainCharacter::SLASH_BITMASK && b->getCollisionBitmask() == MainCharacter::ROPE_MONSTER_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::ROPE_MONSTER_BITMASK && b->getCollisionBitmask() == MainCharacter::SLASH_BITMASK))
	{
		if (a->getCollisionBitmask() == MainCharacter::ROPE_MONSTER_BITMASK)
		{
			m_enemies[a->getGroup()]->GetDamage(MainCharacter::GetInstance()->GetAttack());
		}
		else if (b->getCollisionBitmask() == MainCharacter::ROPE_MONSTER_BITMASK)
		{
			m_enemies[b->getGroup()]->GetDamage(MainCharacter::GetInstance()->GetAttack());
		}
	}

	// MAIN CHARACTER'S ARROW COLLIDE ROPE MONSTER
	if ((a->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK && b->getCollisionBitmask() == MainCharacter::ROPE_MONSTER_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::ROPE_MONSTER_BITMASK && b->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK))
	{
		if (a->getCollisionBitmask() == MainCharacter::ROPE_MONSTER_BITMASK)
		{
			m_enemies[a->getGroup()]->GetDamage(MainCharacter::NORMAL_ARROW);
			MainCharacter::GetInstance()->GetListArrow()[b->getGroup()]->SetVisible(false);
		}
		else if (b->getCollisionBitmask() == MainCharacter::ROPE_MONSTER_BITMASK)
		{
			m_enemies[b->getGroup()]->GetDamage(MainCharacter::NORMAL_ARROW);
			MainCharacter::GetInstance()->GetListArrow()[a->getGroup()]->SetVisible(false);
		}
	}

	
	// BULLET ROPE DAMAGE MAIN CHARACTER
	if ((a->getCollisionBitmask() == MainCharacter::BULLET_ROPE_BITMASK && b->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK && b->getCollisionBitmask() == MainCharacter::BULLET_ROPE_BITMASK))
	{
		MainCharacter::GetInstance()->GetDamage(MainCharacter::ROPE_MONSTER_DAMAGE);
		if (a->getCollisionBitmask() == MainCharacter::BULLET_ROPE_BITMASK)
		{
			m_enemies[a->getGroup()]->GetBullet()->SetVisible(false);
		}
		else if (b->getCollisionBitmask() == MainCharacter::BULLET_ROPE_BITMASK)
		{
			m_enemies[b->getGroup()]->GetBullet()->SetVisible(false);
		}
	}

	// ROPE MONSTER COLLIDE OBSTACLES
	if ((a->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK && b->getCollisionBitmask() == MainCharacter::ROPE_MONSTER_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::ROPE_MONSTER_BITMASK && b->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK))
	{
		if (a->getCollisionBitmask() == MainCharacter::ROPE_MONSTER_BITMASK)
		{
			m_enemies[a->getGroup()]->SetPreventRun();
			m_enemies[a->getGroup()]->ReverseDirection();
		}
		else if (b->getCollisionBitmask() == MainCharacter::ROPE_MONSTER_BITMASK)
		{
			m_enemies[b->getGroup()]->SetPreventRun();
			m_enemies[b->getGroup()]->ReverseDirection();
		}
	}

	//  BULLET MONSTER COLLIDE OBSTACLES

	if ((a->getCollisionBitmask() == MainCharacter::BULLET_ROPE_BITMASK && b->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK && b->getCollisionBitmask() == MainCharacter::BULLET_ROPE_BITMASK))
	{
		if (a->getCollisionBitmask() == MainCharacter::BULLET_ROPE_BITMASK)
		{
			m_enemies[a->getGroup()]->GetBullet()->SetVisible(false);
		}
		else if (b->getCollisionBitmask() == MainCharacter::BULLET_ROPE_BITMASK)
		{
			m_enemies[b->getGroup()]->GetBullet()->SetVisible(false);
		}
	}

	return true;
}

void Level2Scene::NormalAttack(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->NormalAttack();

	}
}

void Level2Scene::SpecialAttack(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->SpecialAttack();
	}
}

void Level2Scene::Evade(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->Evade();
	}
}

void Level2Scene::Defend(Ref* sender, ui::Widget::TouchEventType type)
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

void Level2Scene::OpenInventory(cocos2d::Ref * sender)
{
	MainCharacter::GetInstance()->GetInventory()->AutoArrange();
	GamePlay::ShowInventoryGrid();
	MainCharacter::GetInstance()->GetInventory()->SetVisible(
		!(MainCharacter::GetInstance()->GetInventory()->IsVisible())
	);
}
