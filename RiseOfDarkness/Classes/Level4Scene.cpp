#include "Level4Scene.h"
#include "MainCharacter.h"
#include "ResourceManager.h"
#include "MapScene.h"
#include "HomeScene.h"
#include "Maokai.h"
#include "Elise.h"

using namespace std;

USING_NS_CC;

Scene* Level4Scene::CreateScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = Level4Scene::create();

	scene->addChild(layer);

	return scene;
}

bool Level4Scene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	MainCharacter::GetInstance()->Refresh();

	tileMap = ResourceManager::GetInstance()->GetTileMapById(9);
	upperTileMap = ResourceManager::GetInstance()->GetTileMapById(10);

	CreatePhysicsWorld("ob", "mc", this);

	CreateAllButton(this);

	AddListener();

	CreateMonster();

	auto snow = ParticleSnow::create();
	snow->setPositionY(Director::getInstance()->getVisibleSize().height * 2.2);
	addChild(snow, 12);

	scheduleUpdate();

	return true;
}

void Level4Scene::update(float deltaTime)
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

	if (CheckClear())
	{
		if (MainCharacter::GetInstance()->GetStageLevel() == currentStage)
		{
			Director::getInstance()->pause();
			MainCharacter::GetInstance()->IncreaseStage();
			clear->setVisible(true);
			m_buttons[4]->setVisible(false);
			m_buttons[5]->setVisible(false);
			m_buttons[6]->setVisible(true);
			m_buttons[7]->setVisible(true);
		}
	}

	if (!MainCharacter::GetInstance()->IsAlive())
	{
		m_buttons[4]->setVisible(false);
		m_buttons[5]->setVisible(false);
		m_buttons[6]->setVisible(true);
		m_buttons[7]->setVisible(true);
		gameover->setVisible(true);
	}
	gold->setString(std::to_string(MainCharacter::GetInstance()->GetGold()));
}

void Level4Scene::CreateMonster()
{
	float x1, y1;
	int direction1;
	auto maokaiGroup = tileMap->getObjectGroup("maokai");
	int amount1 = 3;
	char str1[10];
	for (int i = 1; i <= amount1; i++)
	{
		sprintf(str1, "%02d", i);
		x1 = maokaiGroup->getObject(str1)["x"].asFloat();
		y1 = maokaiGroup->getObject(str1)["y"].asFloat();
		Maokai *maokai = new Maokai(this, Vec2(x1, y1), i - 1);
		maokai->GetPhysicsBody()->setGroup(i - 1);
		m_enemies.push_back(maokai);
	}

	float x2, y2;
	int direction2;
	auto eliseGroup = tileMap->getObjectGroup("elise");
	int amount2 = 5;
	char str2[10];
	for (int i = 1; i <= amount2; i++)
	{
		sprintf(str2, "%02d", i);
		direction2 = eliseGroup->getObject(str2)["direction"].asInt();
		x2 = eliseGroup->getObject(str2)["x"].asFloat();
		y2 = eliseGroup->getObject(str2)["y"].asFloat();
		Elise *elise = new Elise(this, direction2, Vec2(x2, y2), i - 1 + amount1);
		elise->GetPhysicsBody()->setGroup(i - 1 + amount1);
		m_enemies.push_back(elise);
	}
}

void Level4Scene::CreatePhysicsWorld(const char * obstacle, const char * mc, Layer * layer)
{
	// Create camera
	camera = Camera::create();
	layer->addChild(camera, 10);

	// Add tile map
	tileMap->removeFromParent();
	tileMap->setPosition(Vec2(0, 0));
	layer->addChild(tileMap, 0);

	// Create physics body world
	auto edgeBody = PhysicsBody::createEdgeBox(tileMap->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
	edgeBody->setCollisionBitmask(MainCharacter::OBSTACLE_BITMASK);
	edgeBody->setContactTestBitmask(true);
	auto edgeNode = Node::create();
	edgeNode->setPosition(tileMap->getContentSize() / 2);
	edgeNode->setPhysicsBody(edgeBody);
	layer->addChild(edgeNode);

	// Create box 2d for obstacles
	auto ob = tileMap->getLayer(obstacle);
	ob->setVisible(false);
	Size layerSize = ob->getLayerSize();
	for (int i = 0; i < layerSize.width; i++)
	{
		for (int j = 0; j < layerSize.height; j++)
		{
			auto tileSet = ob->getTileAt(Vec2(i, j));
			if (tileSet != NULL)
			{
				auto physics = PhysicsBody::createBox(tileSet->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
				physics->setDynamic(false);
				physics->setCollisionBitmask(MainCharacter::OBSTACLE_BITMASK);
				physics->setContactTestBitmask(true);
				tileSet->setPhysicsBody(physics);
			}
		}
	}

	// Add upper tile map
	upperTileMap->removeFromParent();
	upperTileMap->setPosition(Vec2(0, 0));
	layer->addChild(upperTileMap, 3);

	// Add main character
	mainCharacter = MainCharacter::GetInstance()->GetSprite();
	mainCharacter->removeFromParent();
	MainCharacter::GetInstance()->AddToLayer(layer);
	auto obj = tileMap->objectGroupNamed(mc);
	float x = obj->getObject(mc)["x"].asFloat();
	float y = obj->getObject(mc)["y"].asFloat();
	mainCharacter->setPosition(x, y);

	// Get body
	body = MainCharacter::GetInstance()->GetPhysicsBody();
}

void Level4Scene::AddListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Level4Scene::OnTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Level4Scene::OnTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Level4Scene::OnTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	m_buttons[0]->addTouchEventListener(CC_CALLBACK_2(Level4Scene::SpecialAttack, this));
	m_buttons[1]->addTouchEventListener(CC_CALLBACK_2(Level4Scene::Evade, this));
	m_buttons[2]->addTouchEventListener(CC_CALLBACK_2(Level4Scene::NormalAttack, this));
	m_buttons[3]->addTouchEventListener(CC_CALLBACK_2(Level4Scene::Defend, this));

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
			Director::getInstance()->replaceScene(Level4Scene::CreateScene());
		});
		runAction(gotoMap);
	});

	m_buttons[9]->addClickEventListener([&](Ref* event) {
		MainCharacter::GetInstance()->GetInventory()->RemoveItem(0, 0);
	});

	m_buttons[10]->addClickEventListener([&](Ref* event) {
		MainCharacter::GetInstance()->GetInventory()->RemoveItem(1, 1);
	});

	m_buttons[11]->addClickEventListener(CC_CALLBACK_1(Level4Scene::OpenInventory, this));

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Level4Scene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool Level4Scene::OnTouchBegan(Touch* touch, Event* event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::MOVED;
	mCurrentTouchPoint = touch->getLocation();
	auto distance = camera->getPosition() - Director::getInstance()->getVisibleSize() / 2;
	mNextTouchPoint.x = mCurrentTouchPoint.x + distance.x;
	mNextTouchPoint.y = mCurrentTouchPoint.y + distance.y;
	return true;
}

bool Level4Scene::OnTouchEnded(Touch* touch, Event* event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::ENDED;
	mCurrentTouchPoint = Point(-1, -1);
	return true;
}

void Level4Scene::OnTouchMoved(Touch* touch, Event* event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::MOVED;
	mCurrentTouchPoint = touch->getLocation();
	auto distance = camera->getPosition() - Director::getInstance()->getVisibleSize() / 2;
	mNextTouchPoint.x = mCurrentTouchPoint.x + distance.x;
	mNextTouchPoint.y = mCurrentTouchPoint.y + distance.y;
}

bool Level4Scene::onContactBegin(PhysicsContact& contact)
{
	// MAIN CHARACTER WITH OBSTACLES
	Collision(contact, MainCharacter::MAIN_CHARACTER_BITMASK, MainCharacter::OBSTACLE_BITMASK, 1);

	// MAIN CHARACTER WITH MAOKAI
	Collision(contact, MainCharacter::MAIN_CHARACTER_BITMASK, MainCharacter::MAOKAI_MONSTER_BITMASK, 1);

	// MAIN CHARACTER WITH ELISE
	Collision(contact, MainCharacter::MAIN_CHARACTER_BITMASK, MainCharacter::ELISE_MONSTER_BITMASK, 1);

	// ARROW COLLIDE WITH OBSTACLE
	Collision(contact, MainCharacter::NORMAL_ARROW_BITMASK, MainCharacter::OBSTACLE_BITMASK, 2);

	// MAIN CHARACTER SLASH MAOKAI MONSTER
	Collision(contact, MainCharacter::SLASH_BITMASK, MainCharacter::MAOKAI_MONSTER_BITMASK, 3);

	// MAIN CHARACTER SLASH ELISE
	Collision(contact, MainCharacter::SLASH_BITMASK, MainCharacter::ELISE_MONSTER_BITMASK, 3);

	// MAIN CHARACTER'S ARROW COLLIDE MAOKAI MONSTER
	Collision(contact, MainCharacter::NORMAL_ARROW_BITMASK, MainCharacter::MAOKAI_MONSTER_BITMASK, 4);

	// MAIN CHARACTER'S ARROW COLLIDE ELISE
	Collision(contact, MainCharacter::NORMAL_ARROW_BITMASK, MainCharacter::ELISE_MONSTER_BITMASK, 4);

	// FIRE DAMAGE MAIN CHARACTER
	Collision(contact, MainCharacter::FIRE_BITMASK, MainCharacter::MAIN_CHARACTER_BITMASK, 8);

	// BULLET DAMAGE MAIN CHARACTER
	Collision(contact, MainCharacter::BULLET_BITMASK, MainCharacter::MAIN_CHARACTER_BITMASK, 8);

	// BULLET COLLIDE OBSTACLES
	Collision(contact, MainCharacter::BULLET_BITMASK, MainCharacter::OBSTACLE_BITMASK, 7);

	return true;
}

void Level4Scene::NormalAttack(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->NormalAttack();

	}
}

void Level4Scene::SpecialAttack(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->SpecialAttack();
	}
}

void Level4Scene::Evade(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->Evade();
	}
}

void Level4Scene::Defend(Ref* sender, ui::Widget::TouchEventType type)
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

void Level4Scene::Collision(PhysicsContact & contact, int bitmask1, int bitmask2, int type)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	switch (type)
	{
	case 1:
		// MAINCHARACTER COLLISION
		if ((a->getCollisionBitmask() == bitmask1 && b->getCollisionBitmask() == bitmask2)
			|| (a->getCollisionBitmask() == bitmask2 && b->getCollisionBitmask() == bitmask1))
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
		break;
	case 2:
		// MAIN CHARACTER'S ARROW COLLIDE WITH OBSTACLE
		if ((a->getCollisionBitmask() == bitmask1 && b->getCollisionBitmask() == bitmask2)
			|| (a->getCollisionBitmask() == bitmask2 && b->getCollisionBitmask() == bitmask1))
		{
			if (a->getCollisionBitmask() == bitmask1)
			{
				MainCharacter::GetInstance()->GetListArrow()[a->getGroup()]->SetVisible(false);
			}
			else if (b->getCollisionBitmask() == bitmask1)
			{
				MainCharacter::GetInstance()->GetListArrow()[b->getGroup()]->SetVisible(false);
			}
		}
		break;
	case 3:
		// MAIN CHARACTER SLASH COLLIDE WITH MONSTER
		if ((a->getCollisionBitmask() == bitmask1 && b->getCollisionBitmask() == bitmask2)
			|| (a->getCollisionBitmask() == bitmask2 && b->getCollisionBitmask() == bitmask1))
		{
			if (a->getCollisionBitmask() == bitmask2)
			{
				m_enemies[a->getGroup()]->GetDamage(MainCharacter::GetInstance()->GetAttack());
			}
			else if (b->getCollisionBitmask() == bitmask2)
			{
				m_enemies[b->getGroup()]->GetDamage(MainCharacter::GetInstance()->GetAttack());
			}
		}
		break;
	case 4:
		// MAIN CHARACTER'S ARROW COLLIDE MONSTER
		if ((a->getCollisionBitmask() == bitmask1 && b->getCollisionBitmask() == bitmask2)
			|| (a->getCollisionBitmask() == bitmask2 && b->getCollisionBitmask() == bitmask1))
		{
			if (a->getCollisionBitmask() == bitmask2)
			{
				m_enemies[a->getGroup()]->GetDamage(MainCharacter::NORMAL_ARROW);
				MainCharacter::GetInstance()->GetListArrow()[b->getGroup()]->SetVisible(false);
			}
			else if (b->getCollisionBitmask() == bitmask2)
			{
				m_enemies[b->getGroup()]->GetDamage(MainCharacter::NORMAL_ARROW);
				MainCharacter::GetInstance()->GetListArrow()[a->getGroup()]->SetVisible(false);
			}
		}
		break;
	case 5:
		// MONSTERS COLLIDE OBSTACLES
		if ((a->getCollisionBitmask() == bitmask1 && b->getCollisionBitmask() == bitmask2)
			|| (a->getCollisionBitmask() == bitmask2 && b->getCollisionBitmask() == bitmask1))
		{
			if (a->getCollisionBitmask() == bitmask2)
			{
				m_enemies[a->getGroup()]->SetPreventRun();
				m_enemies[a->getGroup()]->ReverseDirection();
			}
			else if (b->getCollisionBitmask() == bitmask2)
			{
				m_enemies[b->getGroup()]->SetPreventRun();
				m_enemies[b->getGroup()]->ReverseDirection();
			}
		}
		break;
	case 6:
		// BULLET DAMAGE MAIN CHARACTER
		if ((a->getCollisionBitmask() == bitmask1 && b->getCollisionBitmask() == bitmask2)
			|| (a->getCollisionBitmask() == bitmask2 && b->getCollisionBitmask() == bitmask1))
		{
			MainCharacter::GetInstance()->GetDamage(MainCharacter::BULLET_DAMAGE);
			if (a->getCollisionBitmask() == bitmask1)
			{
				m_enemies[a->getGroup()]->GetBullet()->SetVisible(false);
			}
			else if (b->getCollisionBitmask() == bitmask1)
			{
				m_enemies[b->getGroup()]->GetBullet()->SetVisible(false);
			}
		}
		break;
	case 7:
		//// BULLET COLLIDE OBSTACLES
		if ((a->getCollisionBitmask() == bitmask1 && b->getCollisionBitmask() == bitmask2)
			|| (a->getCollisionBitmask() == bitmask2 && b->getCollisionBitmask() == bitmask1))
		{
			if (a->getCollisionBitmask() == bitmask1)
			{
				m_enemies[a->getGroup()]->GetBullet()->SetVisible(false);
			}
			else if (b->getCollisionBitmask() == bitmask1)
			{
				m_enemies[b->getGroup()]->GetBullet()->SetVisible(false);
			}
		}
		break;
	case 8:
		// FIRE DAMAGE MAIN CHARACTER
		if ((a->getCollisionBitmask() == bitmask1 && b->getCollisionBitmask() == bitmask2)
			|| (a->getCollisionBitmask() == bitmask2 && b->getCollisionBitmask() == bitmask1))
		{
			MainCharacter::GetInstance()->GetDamage(MainCharacter::FIRE_DAMAGE);
			if (a->getCollisionBitmask() == bitmask1)
			{
				m_enemies[a->getGroup()]->GetFire()->SetVisible(false);
			}
			else if (b->getCollisionBitmask() == bitmask1)
			{
				m_enemies[b->getGroup()]->GetFire()->SetVisible(false);
			}
		}
		break;
	default:
		break;
	}
}

void Level4Scene::OpenInventory(cocos2d::Ref * sender)
{
	MainCharacter::GetInstance()->GetInventory()->AutoArrange();
	GamePlay::ShowInventoryGrid();
	MainCharacter::GetInstance()->GetInventory()->SetVisible(
		!(MainCharacter::GetInstance()->GetInventory()->IsVisible())
	);
}
