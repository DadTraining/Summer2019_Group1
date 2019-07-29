#include "HomeScene.h"
#include "MapScene.h"
#include "MainCharacter.h"
#include "ResourceManager.h"

USING_NS_CC;

Scene* HomeScene::CreateScene()
{
	auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

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
	
	auto layer = Layer::create();
	
	CreatePhysicsWorld("obstacles", "mc", this);

	CreateAllButton(this);
	
	AddListener();

	CreateNPC();

	scheduleUpdate();


	return true;
}

void HomeScene::update(float deltaTime)
{
	UpdateInfoBar();

	MainCharacter::GetInstance()->Update(deltaTime);

	SetCamera(mainCharacter->getPosition());

	RunActionNPC();

	UpdateJoystick();
}

void HomeScene::AddListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HomeScene::OnTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HomeScene::OnTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(HomeScene::OnTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	m_buttons[1]->addClickEventListener([&](Ref* event) {
		auto gotoMapScene = CallFunc::create([] {
			Director::getInstance()->replaceScene(MapScene::CreateScene());
		});
		runAction(gotoMapScene);
	});

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HomeScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	m_buttons[2]->addClickEventListener(CC_CALLBACK_1(HomeScene::OpenInventory, this));
}

bool HomeScene::OnTouchBegan(Touch* touch, Event* event)
{
	
	return true;
}

bool HomeScene::OnTouchEnded(Touch* touch, Event* event)
{
	
	return true;
}

void HomeScene::OnTouchMoved(Touch* touch, Event* event)
{
	
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
	GamePlay::ShowInventoryGrid();
	MainCharacter::GetInstance()->GetInventory()->SetVisible(
		!(MainCharacter::GetInstance()->GetInventory()->IsVisible())
	);
}

void HomeScene::CreateAllButton(Layer* layer)
{
	anchorCamera = Camera::create();
	anchorCamera->setCameraFlag(CameraFlag::USER1);

	layer->addChild(anchorCamera);

	auto get = ResourceManager::GetInstance();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	// CREATE JOYSTICK
	CreateJoystick(layer);

	// STATUS MC ID 0
	auto mainCharacterFace = get->GetButtonById(25);
	mainCharacterFace->setAnchorPoint(Vec2(0, 1));
	mainCharacterFace->removeFromParent();
	mainCharacterFace->setPosition(Vec2(0, visibleSize.height));
	layer->addChild(mainCharacterFace, 4);
	mainCharacterFace->setCameraMask(2);
	m_buttons.push_back(mainCharacterFace);

	// NAME'S MAIN CHARACTER TEXT
	mName = get->GetLabelById(0);
	mName->setString(MainCharacter::GetInstance()->GetName());
	mName->removeFromParent();
	mName->setPosition(Vec2(mainCharacterFace->getBoundingBox().size.width + 10
		, visibleSize.height - mainCharacterFace->getBoundingBox().size.height / 2));
	mName->setAnchorPoint(Vec2(0, 0.5));
	mName->setCameraMask(2);
	layer->addChild(mName, 4);

	// SPRITE ID 0
	auto infoBar = get->GetSpriteById(11);
	infoBar->setAnchorPoint(Vec2(0, 1));
	infoBar->removeFromParent();
	infoBar->setPosition(Vec2(0, visibleSize.height - mainCharacterFace->getBoundingBox().size.height));
	layer->addChild(infoBar, 4);
	infoBar->setCameraMask(2);
	m_sprites.push_back(infoBar);
	auto infoBarPosition = infoBar->getPosition();
	auto infoBarSize = infoBar->getBoundingBox().size;

	//SPRITE ID 1
	auto hpBar = get->GetSpriteById(12);
	hpBar->removeFromParent();
	layer->addChild(hpBar, 5);
	hpBar->setPosition(Vec2(infoBar->getPositionX() / 2 + infoBarSize.width / 1.6, infoBar->getPositionY() - infoBarSize.height / 2.7));
	m_sprites.push_back(hpBar);
	hpBar->setCameraMask(2);

	//SPRITE ID 2
	auto mpBar = get->GetSpriteById(13);
	mpBar->removeFromParent();
	layer->addChild(mpBar, 5);
	mpBar->setPosition(Vec2(infoBar->getPositionX() / 2 + infoBarSize.width / 1.6, infoBar->getPositionY() - infoBarSize.height / 1.5));
	m_sprites.push_back(mpBar);
	mpBar->setCameraMask(2);

	hpLoadingBar = get->GetLoadingbar(1);
	hpLoadingBar->removeFromParent();
	hpLoadingBar->setPosition(hpBar->getPosition());
	layer->addChild(hpLoadingBar, 6);
	hpLoadingBar->setCameraMask(2);

	mpLoadingBar = get->GetLoadingbar(2);
	mpLoadingBar->removeFromParent();
	mpLoadingBar->setPosition(mpBar->getPosition());
	layer->addChild(mpLoadingBar, 6);
	mpLoadingBar->setCameraMask(2);

	//GOLD FRAME SPRITE ID 3 
	auto goldFrame = get->GetSpriteById(16);
	goldFrame->setAnchorPoint(Vec2(1, 1));
	goldFrame->removeFromParent();
	goldFrame->setPosition(Vec2(visibleSize.width / 1.5, visibleSize.height));
	layer->addChild(goldFrame, 4);
	m_sprites.push_back(goldFrame);
	goldFrame->setCameraMask(2);

	// AMOUNT OF GOLD'S MAINCHARACTER
	gold = get->GetLabelById(4);
	gold->removeFromParent();
	gold->setAnchorPoint(Vec2(1, 1));
	gold->setColor(Color3B::YELLOW);
	gold->setPosition(Vec2(visibleSize.width / 1.5, visibleSize.height
		- goldFrame->getBoundingBox().size.height / 2 + gold->getBoundingBox().size.height / 2));
	gold->setString(std::to_string(MainCharacter::GetInstance()->GetGold()));
	layer->addChild(gold, 5);
	gold->setCameraMask(2);

	// FOCUS SPRITE ID 4
	auto focus = get->GetSpriteById(28);
	focus->removeFromParent();
	layer->addChild(focus, 4);
	m_sprites.push_back(focus);

	// DIRECTION SPRITE ID 5
	auto direction = get->DuplicateSprite(get->GetSpriteById(14));
	direction->setScale(2);
	layer->addChild(direction, 4);
	direction->setPosition(Vec2(visibleSize.width - 20, visibleSize.height - 20));
	direction->setCameraMask(2);
	direction->setRotation(-90);
	m_sprites.push_back(direction);

	// GO TO MAP BUTTON ID 1
	auto map = get->GetButtonById(14);
	map->removeFromParent();
	layer->addChild(map, 4);
	map->setCameraMask(2);
	map->setPosition(Vec2(visibleSize.width / 2, 40));
	m_buttons.push_back(map);

	//BUTTON OPEN INVENTORY 2
	auto buttonOpenInventory = ui::Button::create("res/sprites/item/inventory.png");
	buttonOpenInventory->retain();
	layer->addChild(buttonOpenInventory, 4);
	buttonOpenInventory->setAnchorPoint(Vec2(0.5, 0));
	buttonOpenInventory->setPosition(Vec2(visibleSize.width / 2 - map->getBoundingBox().size.width * 2, 40));
	m_buttons.push_back(buttonOpenInventory);
	buttonOpenInventory->setCameraMask(2);
	MainCharacter::GetInstance()->GetInventory()->SetSpritePosition(Vec2(visibleSize / 2));

	SetCamera(MainCharacter::GetInstance()->GetSprite()->getPosition());
}

void HomeScene::CreateNPC()
{
	weaponSeller = ResourceManager::GetInstance()->GetSpriteById(20);
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

	equipmentSeller = ResourceManager::GetInstance()->GetSpriteById(19);
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
	
	potionSeller = ResourceManager::GetInstance()->GetSpriteById(21);
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
	m_sprites[6]->setVisible(true);
	m_buttons[3]->setVisible(true);
	weaponscrollView->setVisible(true);
	
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
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// WEAPON SHOP
	// SPRITE ID 6
	auto shopweapon = Sprite::create("res/sprites/shop/wea_shop.png");
	shopweapon->setPosition(visibleSize / 2);
	addChild(shopweapon, 4);
	shopweapon->setVisible(false);
	shopweapon->setCameraMask(2);
	m_sprites.push_back(shopweapon);

	// BUTTON ID 3
	auto close = ui::Button::create("res/buttons/shopp/xx.png", "res/buttons/shopp/xx_bg.png");
	close->setPosition(Vec2(visibleSize.width / 2 + shopweapon->getBoundingBox().size.width / 2
		, visibleSize.height / 2 + shopweapon->getBoundingBox().size.height / 2));
	addChild(close, 60);
	close->setCameraMask(2);
	close->setVisible(false);
	m_buttons.push_back(close);

	weaponscrollView = ui::ScrollView::create();
	weaponscrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	weaponscrollView->setContentSize(Size(385, 220));
	weaponscrollView->setInnerContainerSize(Size(400, 1560));
	weaponscrollView->setBounceEnabled(true);
	weaponscrollView->setTouchEnabled(true);
	weaponscrollView->setPosition(Vec2(510, 360));
	weaponscrollView->setVisible(false);
	weaponscrollView->setCameraMask(2);
	this->addChild(weaponscrollView, 70);

	close->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			shopweapon->setVisible(false);
			close->setVisible(false);
			weaponscrollView->setVisible(false);
		}
	});
	


	//EQUIPMENT SHOP


	//POTION SHOP




}
