#include "HomeScene.h"
#include "ResourceManager.h"
#include "MainCharacter.h"
#include "MapScene.h"

using namespace std;
USING_NS_CC;
using namespace ui;
ui::ScrollView* inventory;
Menu *menu;
vector<ui::Button*> listSprite;
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
	
	CreatePhysicsWorld("obstacles", "mc", this);

	CreateAllButton(this);
	
	AddListener();
	
	
	scheduleUpdate();

	return true;
}

void HomeScene::update(float deltaTime)
{
	UpdateController();

	MainCharacter::GetInstance()->Update(deltaTime);

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
		MainCharacter::GetInstance()->SpecialAttack();
	}
}

void HomeScene::SpecialAttack(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->NormalAttack();
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

void HomeScene::CreateAllButton(Layer* layer)
{
	auto get = ResourceManager::GetInstance();
	// add button for test
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto btnBag = ui::Button::create("res/sprites/item/inventory.png");
	btnBag->setPosition(Vec2(visibleSize.width*0.4, btnBag->getContentSize().height));
	layer->addChild(btnBag,5);
	btnBag->addClickEventListener(CC_CALLBACK_1(HomeScene::openInventory, this));

	auto tab = ui::TabControl::create();
	tab->setContentSize(Size(200, 200));
	tab->setHeaderHeight(20);
	tab->setHeaderWidth(70);
	tab->setHeaderDockPlace(ui::TabControl::Dock::TOP);

	auto tabWeapon = ui::TabHeader::create();
	tabWeapon->setTitleText("Weapon");
	auto tabArmor = ui::TabHeader::create();
	tabArmor->setTitleText("Armor");
	auto tabPotion = ui::TabHeader::create();
	tabPotion->setTitleText("Potion");
	
	auto container1 = Layout::create();
	container1->setOpacity(255);
	container1->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	container1->setBackGroundColor(Color3B::GRAY);
	container1->setBackGroundColorOpacity(255);
	auto container2 = Layout::create();
	container2->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	container2->setOpacity(255);
	container2->setBackGroundColor(Color3B::BLUE);
	container2->setBackGroundColorOpacity(255);
	auto container3 = Layout::create();
	container3->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	container3->setOpacity(255);
	container3->setBackGroundColor(Color3B::RED);
	container3->setBackGroundColorOpacity(255);

	tab->insertTab(0, tabWeapon, container1);
	tab->insertTab(1, tabArmor, container2);
	tab->insertTab(2, tabPotion, container3);

	tab->setSelectTab(2);
	layer->addChild(tab,9);
	tab->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	inventory = ui::ScrollView::create();
	//inventory->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	//inventory->setBackGroundColor(Color3B::BLUE);
	inventory->setContentSize(Size(280, 200));
	inventory->setAnchorPoint(Point(0, 1));
	inventory->setPosition(Point(visibleSize.width / 2, visibleSize.height*0.8));
	auto containerSize = Size(inventory->getContentSize().width,
		inventory->getContentSize().height * 2);
	log("size : %f,%f", containerSize.width, containerSize.height);
	inventory->setInnerContainerSize(containerSize);
	inventory->setInnerContainerPosition(Vec2(0, inventory->getContentSize().height));
	//inventory->getInnerContainer()->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	//inventory->getInnerContainer()->setBackGroundColor(Color3B::WHITE);
	inventory->getInnerContainer()->setAnchorPoint(Point(0, 1));
	inventory->getInnerContainer()->setBackGroundColorOpacity(100);
	inventory->setScrollBarEnabled(false);
	inventory->setVisible(false);

	layer->addChild(inventory, 10);
	//init item
	for (int i = 1; i <= 30; i++)
	{
		string normal = "res/sprites/item/items_" + (i < 10 ? "0" + to_string(i) : to_string(i)) + ".png";
		string pressed = "res/sprites/item/items_" + (i < 10 ? "0" + to_string(i) : to_string(i)) + ".png";
		auto item0 = ui::Button::create(normal, pressed);
		item0->addClickEventListener(CC_CALLBACK_1(HomeScene::itemCallback, this, i - 1));
		listSprite.push_back(item0);
	}

	container1->addChild(listSprite[0], 11);

	auto box = Sprite::create("res/sprites/item/grid.png");
	box->setAnchorPoint(Vec2(0, 0));
	box->setScale(1);
	box->setPosition(0,0);
	//inventory->addChild(box, 10);
	//set item position to show
	int col = inventory->getContentSize().width / 32;
	int row = 1, j = 0;
	
	for (int i = 0; i < listSprite.size(); i++)
	{
		listSprite[i]->setAnchorPoint(Point(0.5, 0.5));
		listSprite[i]->setPosition(Vec2(j * 32 + inventory->getInnerContainerPosition().x,
			inventory->getInnerContainerPosition().y * 2 - row * 32));
		j++;
		if (j == col)
		{
			j = 0;
			row++;
		}
		
		//inventory->addChild(listSprite[i], 11);
		//log("item[%d] : %f,%f", i, listSprite[i]->getPositionX(), listSprite[i]->getPositionY());
	}

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

	// GO TO MAP BUTTON
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

	auto mainCharacterFace = get->GetSpriteById(19);
	mainCharacterFace->setAnchorPoint(Vec2(0, 1));
	mainCharacterFace->removeFromParent();
	layer->addChild(mainCharacterFace, 8);
	m_sprites.push_back(mainCharacterFace);

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
	hpLoadingBar->setPercent(50);
	layer->addChild(hpLoadingBar, 10);

	mpLoadingBar = get->GetLoadingbar(2);
	mpLoadingBar->removeFromParent();
	mpLoadingBar->setPercent(50);
	layer->addChild(mpLoadingBar, 10);

	SetCamera(mainCharacter->getPosition());
}

//open inventory
void HomeScene::openInventory(cocos2d::Ref *sender)
{
	inventory->setVisible(!inventory->isVisible());
}
void HomeScene::itemCallback(cocos2d::Ref *pSender, int item)
{
	auto btnEquip = MenuItemImage::create("res/sprites/item/btnEquip.png", "res/sprites/item/btnEquip.png",
		CC_CALLBACK_1(HomeScene::btnEquipInventory, this, item));
	auto btnDrop = MenuItemImage::create("res/sprites/item/btnDrop.png", "res/sprites/item/btnDrop.png");
	auto btnBack = MenuItemImage::create("res/sprites/item/btnDrop.png", "res/sprites/item/btnDrop.png",
		CC_CALLBACK_1(HomeScene::btnBackInventory, this));

	btnDrop->setScale(0.5);
	btnEquip->setScale(0.5);
	btnDrop->setPositionY(btnEquip->getPositionY() - btnEquip->getContentSize().height / 2);
	menu = Menu::create(btnEquip, btnDrop, btnBack, NULL);
	menu->setAnchorPoint(Vec2(0, 1));
	menu->setVisible(false);

	btnBack->setPositionY(btnDrop->getPositionY() - btnDrop->getContentSize().height / 2);
	btnBack->setScale(0.5);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	switch (item)
	{
	case 0:
		log("item 1 clicked!");
		menu->setVisible(!menu->isVisible());
		menu->setPosition(listSprite[item]->getPosition() + Vec2(inventory->getPositionX(), 0));
		addChild(menu, 99);
		break;
	case 1:
		log("item 2 clicked!");
		menu->setVisible(!menu->isVisible());
		menu->setPosition(listSprite[item]->getPosition() + Vec2(inventory->getPositionX(), 0));
		addChild(menu, 99);
		break;
	default:
		break;
	}
}

void HomeScene::btnBackInventory(cocos2d::Ref *)
{
	menu->setVisible(!menu->isVisible());
}

void HomeScene::btnEquipInventory(cocos2d::Ref *, int id)
{
	log("Equiped");
	inventory->removeChild(listSprite[id]);
	log("removed this");
	menu->setVisible(false);
}

//
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

	m_sprites[10]->setPosition(pos.x - visibleSize.width / 2, pos.y + visibleSize.height / 2);
	mName->setPosition(pos.x - visibleSize.width / 2 + m_sprites[10]->getBoundingBox().size.width + 10, pos.y + visibleSize.height / 2 - (m_sprites[10]->getBoundingBox().size.height / 2 - mName->getBoundingBox().size.height / 2));
	m_sprites[11]->setPosition(pos.x - visibleSize.width / 2, pos.y + visibleSize.height / 2 - m_sprites[10]->getBoundingBox().size.height);
	auto infoBarPosition = m_sprites[11]->getPosition();
	auto infoBarSize = m_sprites[11]->getBoundingBox().size;
	m_sprites[12]->setPosition(infoBarPosition.x + infoBarSize.width / 1.6, infoBarPosition.y - infoBarSize.height / 2.8);
	m_sprites[13]->setPosition(infoBarPosition.x + infoBarSize.width / 1.6, infoBarPosition.y - infoBarSize.height / 1.5);
	hpLoadingBar->setPosition(m_sprites[12]->getPosition());
	mpLoadingBar->setPosition(m_sprites[13]->getPosition());
}