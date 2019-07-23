#include "HomeScene.h"
#include "ResourceManager.h"
#include "MainCharacter.h"
#include "MapScene.h"

USING_NS_CC;

Scene* HomeScene::CreateScene()
{
	auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	auto layer = HomeScene::create();

	scene->addChild(layer);

	return scene;
}

bool HomeScene::init()
{
	if (!Layer::init())
	{
		return false;
	}


	MainCharacter::GetInstance()->Refresh();

	tileMap = ResourceManager::GetInstance()->GetTileMapById(0);
	upperTileMap = ResourceManager::GetInstance()->GetTileMapById(1);
	
	CreatePhysicsWorld("obstacles", "mc", this);

	CreateAllButton(this);
	
	AddListener();

	CreateNPC();

	scheduleUpdate();

	return true;
}

void HomeScene::update(float deltaTime)
{
	UpdateController();

	UpdateInfoBar();

	MainCharacter::GetInstance()->Update(deltaTime);

	SetCamera(mainCharacter->getPosition());

	RunActionNPC();
}

void HomeScene::AddListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HomeScene::OnTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HomeScene::OnTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(HomeScene::OnTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	m_buttons[0]->addTouchEventListener(CC_CALLBACK_2(HomeScene::SpecialAttack, this));
	m_buttons[1]->addTouchEventListener(CC_CALLBACK_2(HomeScene::Evade, this));
	m_buttons[2]->addTouchEventListener(CC_CALLBACK_2(HomeScene::NormalAttack, this));
	m_buttons[3]->addTouchEventListener(CC_CALLBACK_2(HomeScene::Defend, this));

	m_buttons[4]->addClickEventListener([&](Ref* event) {
		auto gotoMapScene = CallFunc::create([] {
			Director::getInstance()->replaceScene(MapScene::CreateScene());
		});
		runAction(gotoMapScene);
	});

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HomeScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool HomeScene::OnTouchBegan(Touch* touch, Event* event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::MOVED;
	mCurrentTouchPoint = touch->getLocation();
	auto distance = camera->getPosition() - Director::getInstance()->getVisibleSize() / 2;
	mNextTouchPoint.x = mCurrentTouchPoint.x + distance.x;
	mNextTouchPoint.y = mCurrentTouchPoint.y + distance.y;
	return true;
}

bool HomeScene::OnTouchEnded(Touch* touch, Event* event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::ENDED;
	mCurrentTouchPoint = Point(-1, -1);
	return true;
}

void HomeScene::OnTouchMoved(Touch* touch, Event* event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::MOVED;
	mCurrentTouchPoint = touch->getLocation();
	auto distance = camera->getPosition() - Director::getInstance()->getVisibleSize() / 2;
	mNextTouchPoint.x = mCurrentTouchPoint.x + distance.x;
	mNextTouchPoint.y = mCurrentTouchPoint.y + distance.y;
}

void HomeScene::NormalAttack(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->NormalAttack();
	}
}

void HomeScene::SpecialAttack(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->SpecialAttack();
	}
}

void HomeScene::Evade(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->Evade();
	}
}

void HomeScene::Defend(Ref* sender, ui::Widget::TouchEventType type)
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

bool HomeScene::onContactBegin(PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

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
	if (a->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK && b->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK)
	{
		MainCharacter::GetInstance()->GetListArrow().at(a->getGroup())->SetVisible(false);
	}
	else if (a->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK && b->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK)
	{
		MainCharacter::GetInstance()->GetListArrow().at(b->getGroup())->SetVisible(false);
	}

	// MAIN CHARACTER COLLIDE WITH WEAPON SHOP
	if ((a->getCollisionBitmask() == 11 && b->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK && b->getCollisionBitmask() == 11))
	{
		OpenWeaponShop();
	}

	// MAIN CHARACTER COLLIDE WITH EQUIPMENT SHOP
	if ((a->getCollisionBitmask() == 12 && b->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK && b->getCollisionBitmask() == 12))
	{
		OpenEquipmentShop();
	}

	// MAIN CHARACTER COLLIDE WITH POTION SHOP
	if ((a->getCollisionBitmask() == 13 && b->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK && b->getCollisionBitmask() == 13))
	{
		OpenPotionShop();
	}
	return true;
}

void HomeScene::OpenInventory(cocos2d::Ref * sender)
{
	MainCharacter::GetInstance()->GetInventory()->SetVisible(
		!(MainCharacter::GetInstance()->GetInventory()->IsVisible())
	);
}

void HomeScene::CreateAllButton(Layer* layer)
{
	auto get = ResourceManager::GetInstance();

	// SPRITE ID 0
	auto frameButton = get->GetSpriteById(18);
	frameButton->removeFromParent();
	frameButton->setVisible(false);
	layer->addChild(frameButton, 5);
	m_sprites.push_back(frameButton);

	// GO UP SPRITE ID 1
	auto upButton = get->GetSpriteById(10);
	upButton->removeFromParent();
	upButton->setAnchorPoint(Vec2(0.5, 1));
	layer->addChild(upButton, 6);
	m_sprites.push_back(upButton);

	//SPRITE ID 2
	auto upButtonPressed = get->GetSpriteById(11);
	upButtonPressed->removeFromParent();
	upButtonPressed->setAnchorPoint(Vec2(0.5, 1));
	layer->addChild(upButtonPressed, 7);
	upButtonPressed->setVisible(false);
	m_sprites.push_back(upButtonPressed);

	// GO DOWN SPRITE ID 3
	auto downButton = get->GetSpriteById(12);
	downButton->removeFromParent();
	downButton->setAnchorPoint(Vec2(0.5, 0));
	layer->addChild(downButton, 6);
	m_sprites.push_back(downButton);

	//SPRITE ID 4
	auto downButtonPressed = get->GetSpriteById(13);
	downButtonPressed->removeFromParent();
	downButtonPressed->setAnchorPoint(Vec2(0.5, 0));
	layer->addChild(downButtonPressed, 7);
	downButtonPressed->setVisible(false);
	m_sprites.push_back(downButtonPressed);

	// GO LEFT SPRITE ID 5
	auto leftButton = get->GetSpriteById(14);
	leftButton->removeFromParent();
	leftButton->setAnchorPoint(Vec2(0, 0.5));
	layer->addChild(leftButton, 6);
	m_sprites.push_back(leftButton);

	// SPRITE ID 6
	auto leftButtonPressed = get->GetSpriteById(15);
	leftButtonPressed->removeFromParent();
	leftButtonPressed->setAnchorPoint(Vec2(0, 0.5));
	layer->addChild(leftButtonPressed, 7);
	leftButtonPressed->setVisible(false);
	m_sprites.push_back(leftButtonPressed);

	// GO RIGHT SPRITE ID 7
	auto rightButton = get->GetSpriteById(16);
	rightButton->removeFromParent();
	rightButton->setAnchorPoint(Vec2(1, 0.5));
	layer->addChild(rightButton, 6);
	m_sprites.push_back(rightButton);

	// SPRITE ID 8
	auto rightButtonPressed = get->GetSpriteById(17);
	rightButtonPressed->removeFromParent();
	rightButtonPressed->setAnchorPoint(Vec2(1, 0.5));
	layer->addChild(rightButtonPressed, 7);
	rightButtonPressed->setVisible(false);
	m_sprites.push_back(rightButtonPressed);

	// SPRITE ID 9
	auto frameSkillButton = get->DuplicateSprite(get->GetSpriteById(18));
	frameSkillButton->removeFromParent();
	frameSkillButton->setVisible(false);
	layer->addChild(frameSkillButton, 5);
	m_sprites.push_back(frameSkillButton);

	// SPECIAL ATTACK BUTTON ID 0
	auto specialAttack = get->GetButtonById(10);
	specialAttack->removeFromParent();
	specialAttack->setAnchorPoint(Vec2(0.5, 1));
	layer->addChild(specialAttack, 6);
	m_buttons.push_back(specialAttack);

	// EVDAE BUTTON ID 1
	auto evade = get->GetButtonById(11);
	evade->removeFromParent();
	evade->setAnchorPoint(Vec2(0.5, 0));
	layer->addChild(evade, 6);
	m_buttons.push_back(evade);

	// NORMAL ATTACK BUTTON ID 2
	auto normalAttack = get->GetButtonById(12);
	normalAttack->removeFromParent();
	normalAttack->setAnchorPoint(Vec2(0, 0.5));
	layer->addChild(normalAttack, 6);
	m_buttons.push_back(normalAttack);

	// DEFEND BUTTON ID 3
	auto defend = get->GetButtonById(13);
	defend->removeFromParent();
	defend->setAnchorPoint(Vec2(1, 0.5));
	layer->addChild(defend, 6);
	m_buttons.push_back(defend);

	// GO TO MAP BUTTON 4
	auto map = get->GetButtonById(14);
	map->removeFromParent();
	map->setAnchorPoint(Vec2(0.5, 0));
	layer->addChild(map, 7);
	m_buttons.push_back(map);

	mName = get->GetLabelById(0);
	mName->setString(MainCharacter::GetInstance()->GetName());
	mName->removeFromParent();
	mName->setAnchorPoint(Vec2(0, 1));
	layer->addChild(mName, 8);

	// STATUS MC ID 5
	auto mainCharacterFace = get->GetButtonById(25);
	mainCharacterFace->setAnchorPoint(Vec2(0, 1));
	mainCharacterFace->removeFromParent();
	layer->addChild(mainCharacterFace, 8);
	m_buttons.push_back(mainCharacterFace);

	//BUTTON OPEN INVENTORY 6
	auto buttonOpenInventory = ui::Button::create("res/sprites/item/inventory.png");
	buttonOpenInventory->setAnchorPoint(Vec2(0.5, 0));
	buttonOpenInventory->retain();
	layer->addChild(buttonOpenInventory, 7);
	m_buttons.push_back(buttonOpenInventory);

	auto infoBar = get->GetSpriteById(20);
	infoBar->setAnchorPoint(Vec2(0, 1));
	infoBar->removeFromParent();
	layer->addChild(infoBar, 8);
	m_sprites.push_back(infoBar);

	auto hpBar = get->GetSpriteById(21);
	hpBar->removeFromParent();
	layer->addChild(hpBar, 9);
	m_sprites.push_back(hpBar);

	auto mpBar = get->GetSpriteById(22);
	mpBar->removeFromParent();
	layer->addChild(mpBar, 9);
	m_sprites.push_back(mpBar);

	hpLoadingBar = get->GetLoadingbar(1);
	hpLoadingBar->removeFromParent();
	layer->addChild(hpLoadingBar, 10);

	mpLoadingBar = get->GetLoadingbar(2);
	mpLoadingBar->removeFromParent();
	layer->addChild(mpLoadingBar, 10);

	SetCamera(mainCharacter->getPosition());
}

void HomeScene::SetCamera(Vec2 pos)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto tileMapSize = tileMap->getBoundingBox().size;
	float xMin = visibleSize.width / 2, xMax = tileMapSize.width - xMin, yMin = visibleSize.height / 2, yMax = tileMapSize.height - yMin;
	if (pos.x < xMin)
	{
		pos.x = xMin;
	}
	else if (pos.x > xMax)
	{
		pos.x = xMax;
	}
	if (pos.y < yMin)
	{
		pos.y = yMin;
	}
	else if (pos.y > yMax)
	{
		pos.y = yMax;
	}
	camera->setPosition(Vec2(pos.x, pos.y));

	auto distance = camera->getPosition() - Director::getInstance()->getVisibleSize() / 2;
	mNextTouchPoint.x = mCurrentTouchPoint.x + distance.x;
	mNextTouchPoint.y = mCurrentTouchPoint.y + distance.y;

	m_sprites[0]->setPosition(Vec2(pos.x - visibleSize.width / 3, pos.y - visibleSize.height / 5));
	auto frameButtonPosition = m_sprites[0]->getPosition();
	auto frameButtonSize = m_sprites[0]->getBoundingBox().size;
	m_sprites[1]->setPosition(Vec2(frameButtonPosition.x, frameButtonPosition.y + frameButtonSize.height / 1.5));
	m_sprites[2]->setPosition(Vec2(frameButtonPosition.x, frameButtonPosition.y + frameButtonSize.height / 1.5));
	m_sprites[3]->setPosition(Vec2(frameButtonPosition.x, frameButtonPosition.y - frameButtonSize.height / 1.5));
	m_sprites[4]->setPosition(Vec2(frameButtonPosition.x, frameButtonPosition.y - frameButtonSize.height / 1.5));
	m_sprites[5]->setPosition(Vec2(frameButtonPosition.x - frameButtonSize.width / 1.5, frameButtonPosition.y));
	m_sprites[6]->setPosition(Vec2(frameButtonPosition.x - frameButtonSize.width / 1.5, frameButtonPosition.y));
	m_sprites[7]->setPosition(Vec2(frameButtonPosition.x + frameButtonSize.width / 1.5, frameButtonPosition.y));
	m_sprites[8]->setPosition(Vec2(frameButtonPosition.x + frameButtonSize.width / 1.5, frameButtonPosition.y));

	m_sprites[9]->setPosition(Vec2(pos.x + visibleSize.width / 3, pos.y - visibleSize.height / 5));
	auto frameSkillButtonPosition = m_sprites[9]->getPosition();
	auto frameSkillButtonSize = m_sprites[9]->getBoundingBox().size;
	m_buttons[0]->setPosition(Vec2(frameSkillButtonPosition.x, frameSkillButtonPosition.y + frameSkillButtonSize.height / 1.5));
	m_buttons[1]->setPosition(Vec2(frameSkillButtonPosition.x, frameSkillButtonPosition.y - frameSkillButtonSize.height / 1.5));
	m_buttons[2]->setPosition(Vec2(frameSkillButtonPosition.x - frameSkillButtonSize.width / 1.5, frameSkillButtonPosition.y));
	m_buttons[3]->setPosition(Vec2(frameSkillButtonPosition.x + frameSkillButtonSize.width / 1.5, frameSkillButtonPosition.y));

	m_buttons[4]->setPosition(Vec2(pos.x, pos.y - visibleSize.height / 2));
	m_buttons[6]->setPosition(Vec2(pos.x+visibleSize.width/8, pos.y - visibleSize.height / 2));
	m_buttons[6]->addClickEventListener(CC_CALLBACK_1(HomeScene::OpenInventory, this));
	m_buttons[5]->setPosition(Vec2(pos.x - visibleSize.width / 2, pos.y + visibleSize.height / 2));
	mName->setPosition(pos.x - visibleSize.width / 2 + m_buttons[5]->getBoundingBox().size.width + 10, pos.y + visibleSize.height / 2 - (m_buttons[5]->getBoundingBox().size.height / 2 - mName->getBoundingBox().size.height / 2));
	m_sprites[10]->setPosition(pos.x - visibleSize.width / 2, pos.y + visibleSize.height / 2 - m_buttons[5]->getBoundingBox().size.height);
	auto infoBarPosition = m_sprites[10]->getPosition();
	auto infoBarSize = m_sprites[10]->getBoundingBox().size;
	m_sprites[11]->setPosition(infoBarPosition.x + infoBarSize.width / 1.6, infoBarPosition.y - infoBarSize.height / 2.8);
	m_sprites[12]->setPosition(infoBarPosition.x + infoBarSize.width / 1.6, infoBarPosition.y - infoBarSize.height / 1.5);
	hpLoadingBar->setPosition(m_sprites[11]->getPosition());
	mpLoadingBar->setPosition(m_sprites[12]->getPosition());

	//AddChild inventory
	MainCharacter::GetInstance()->GetInventory()->SetSpritePosition(Vec2(pos.x, pos.y));
}

void HomeScene::CreateNPC()
{
	weaponSeller = ResourceManager::GetInstance()->GetSpriteById(29);
	weaponSeller->removeFromParent();
	this->addChild(weaponSeller, 0);
	auto obj1 = tileMap->objectGroupNamed("weaponSeller");
	float x1 = obj1->getObject("weaponSeller")["x"].asFloat();
	float y1 = obj1->getObject("weaponSeller")["y"].asFloat();
	weaponSeller->setPosition(x1, y1);
	weaponSeller->setAnchorPoint(Vec2(0.5, 0));
	weapon = ResourceManager::GetInstance()->GetActionById(32);
	CC_SAFE_RETAIN(weapon);
	auto ob1 = tileMap->getLayer("weaponShop");
	ob1->setVisible(false);
	Size layerSize1 = ob1->getLayerSize();
	for (int i = 0; i < layerSize1.width; i++)
	{
		for (int j = 0; j < layerSize1.height; j++)
		{
			auto tileSet1 = ob1->getTileAt(Vec2(i, j));
			if (tileSet1 != NULL)
			{
				auto physics1 = PhysicsBody::createBox(tileSet1->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
				physics1->setDynamic(false);
				physics1->setCollisionBitmask(11);
				physics1->setContactTestBitmask(true);
				tileSet1->setPhysicsBody(physics1);
			}
		}
	}

	equipmentSeller = ResourceManager::GetInstance()->GetSpriteById(28);
	equipmentSeller->removeFromParent();
	this->addChild(equipmentSeller, 0);
	auto obj2 = tileMap->objectGroupNamed("equipmentSeller");
	float x2 = obj2->getObject("equipmentSeller")["x"].asFloat();
	float y2 = obj2->getObject("equipmentSeller")["y"].asFloat();
	equipmentSeller->setPosition(x2, y2);
	equipmentSeller->setAnchorPoint(Vec2(0.5, 0));
	equipment = ResourceManager::GetInstance()->GetActionById(31);
	CC_SAFE_RETAIN(equipment);
	auto ob2 = tileMap->getLayer("equipmentShop");
	ob2->setVisible(false);
	Size layerSize2 = ob2->getLayerSize();
	for (int i = 0; i < layerSize2.width; i++)
	{
		for (int j = 0; j < layerSize2.height; j++)
		{
			auto tileSet2 = ob2->getTileAt(Vec2(i, j));
			if (tileSet2 != NULL)
			{
				auto physics2 = PhysicsBody::createBox(tileSet2->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
				physics2->setDynamic(false);
				physics2->setCollisionBitmask(12);
				physics2->setContactTestBitmask(true);
				tileSet2->setPhysicsBody(physics2);
			}
		}
	}
	
	potionSeller = ResourceManager::GetInstance()->GetSpriteById(30);
	potionSeller->removeFromParent();
	this->addChild(potionSeller, 0);
	auto obj3 = tileMap->objectGroupNamed("potionSeller");
	float x3 = obj3->getObject("potionSeller")["x"].asFloat();
	float y3 = obj3->getObject("potionSeller")["y"].asFloat();
	potionSeller->setPosition(x3, y3);
	potionSeller->setAnchorPoint(Vec2(0.5, 0));
	potion = ResourceManager::GetInstance()->GetActionById(33);
	CC_SAFE_RETAIN(potion);
	auto ob3 = tileMap->getLayer("potionShop");
	ob3->setVisible(false);
	Size layerSize3 = ob3->getLayerSize();
	for (int i = 0; i < layerSize3.width; i++)
	{
		for (int j = 0; j < layerSize3.height; j++)
		{
			auto tileSet3 = ob3->getTileAt(Vec2(i, j));
			if (tileSet3 != NULL)
			{
				auto physics3 = PhysicsBody::createBox(tileSet3->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
				physics3->setDynamic(false);
				physics3->setCollisionBitmask(13);
				physics3->setContactTestBitmask(true);
				tileSet3->setPhysicsBody(physics3);
			}
		}
	}
	CreateShop();
}

void HomeScene::RunActionNPC()
{
	if (weaponSeller->getNumberOfRunningActions() == 0)
	{
		weaponSeller->runAction(weapon);
	}
	if (equipmentSeller->getNumberOfRunningActions() == 0)
	{
		equipmentSeller->runAction(equipment);
	}
	if (potionSeller->getNumberOfRunningActions() == 0)
	{
		potionSeller->runAction(potion);
	}
}

void HomeScene::OpenWeaponShop()
{
	log("weapon shop");
}

void HomeScene::OpenEquipmentShop()
{
	log("quipment shop");
}

void HomeScene::OpenPotionShop()
{
	log("potion shop");
}

void HomeScene::CreateShop()
{
	// WEAPON SHOP


	//EQUIPMENT SHOP


	//POTION SHOP




}