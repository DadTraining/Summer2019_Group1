#include "HomeScene.h"
#include "ResourceManager.h"
#include "MainCharacter.h"
#include "MapScene.h"

using namespace std;
USING_NS_CC;
using namespace ui;
ui::Layout* layout;
Menu *menu;
vector<ui::Button*> listSprite;
vector<ui::Button*> items;		//store all item sprites
vector<ui::Widget*> itemEquip;  // store items equiped
ui::Layout* equipment;          // show inventory
vector<ui::Button*> itemEquipBox; // show grid inventory
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

void HomeScene::openInventory(cocos2d::Ref *pSender)
{
	layout->isVisible() ? log("close inventory") : log("open inventory");
	//menu->setVisible(!menu->isVisible());
	layout->setVisible(!layout->isVisible());
	equipment->setVisible(!equipment->isVisible());

}

void HomeScene::itemCallback(cocos2d::Ref* pSender, int item)
{
	auto btnEquip = MenuItemImage::create("res/sprites/item/btnEquip.png", "res/sprites/item/btnEquip.png",
		CC_CALLBACK_1(HomeScene::btnEquipInventory, this, item));
	auto btnDrop = MenuItemImage::create("res/sprites/item/btnDrop.png", "res/sprites/item/btnDrop.png");
	auto btnBack = MenuItemImage::create("res/sprites/item/btnClose.png", "res/sprites/item/btnClose.png",
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
	log("item %d clicked!", item);
	menu->setVisible(!menu->isVisible());
	menu->setPosition(items[item]->getPosition() + Vec2(layout->getPositionX(), 0));
	addChild(menu, 99);
}

void HomeScene::btnBackInventory(cocos2d::Ref *)
{
	menu->setVisible(!menu->isVisible());
}

void HomeScene::btnEquipInventory(cocos2d::Ref *, int id)
{
	log("Equiped");
	string path = "res/sprites/item/items_weapon_" + (id + 1 < 10 ? "0" + to_string(id + 1) : to_string(id + 1)) + ".png";
	auto item = ui::Button::create(path);
	item->retain();
	itemEquip.push_back(item);
	layout->removeChild(items[id]);
	log("removed this");
	menu->setVisible(false);
	if (itemEquip.size()>0)
	{
		//remove child and add it again
		for (int i = 0; i < itemEquip.size() - 1; i++)
		{
			equipment->removeChild(itemEquip[i]);
		}
		int max_cols = 2;
		int cols = 0;
		int rows = 0;
		for (int i = 0; i < itemEquip.size(); i++)
		{
			if (cols == max_cols)
			{
				rows++;
				cols = 0;
			}
			itemEquip[i]->setAnchorPoint(Vec2(0, 1));
			itemEquip[i]->setPosition(itemEquipBox[i]->getPosition() + Vec2(16, -16));
			equipment->addChild(itemEquip[i], 16);
			cols++;
		}
	}
}

void HomeScene::CreateAllButton(Layer* layer)
{
	auto get = ResourceManager::GetInstance();
	// add button for test
	//auto visibleSize = Director::getInstance()->getVisibleSize();
	//for (int i = 0; i < 4; i++)
	//{
	//	auto btn = ui::Button::create("res/sprites/item/box.png");
	//	btn->setEnabled(false);
	//	itemEquipBox.push_back(btn);
	//}

	//equipment = ui::Layout::create();
	//equipment->setContentSize(Size(128, 128));
	//equipment->setPosition(Vec2(equipment->getContentSize().width / 2,
	//	visibleSize.height / 2 - equipment->getContentSize().height / 2));
	//equipment->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	//equipment->setBackGroundColor(Color3B::GREEN);
	//equipment->setVisible(false);
	//addChild(equipment,15);

	//int max_cols = equipment->getContentSize().width / 64;

	//int cols = 0, rows = 0;
	//for (int i = 0; i < itemEquipBox.size(); i++)
	//{
	//	if (cols == max_cols)
	//	{
	//		rows++;
	//		cols = 0;
	//	}
	//	itemEquipBox[i]->setAnchorPoint(Vec2(0, 1));
	//	//itemEquip[i]->setAnchorPoint(Vec2(0, 1));
	//	itemEquipBox[i]->setPosition(Vec2(cols * 64, equipment->getContentSize().height - 64 * rows));
	//	//itemEquip[i]->setPosition(itemEquipBox[i]->getPosition() + Vec2(16, -16));
	//	equipment->addChild(itemEquipBox[i], 16);
	//	//equipment->addChild(itemEquip[i], 6);
	//	cols++;
	//}

	//vector<ui::Button*> btnList;	//grid item
	//for (int i = 1; i <= 20; i++)
	//{
	//	string path = "res/sprites/item/items_weapon_" + (i < 10 ? "0" + to_string(i) : to_string(i)) + ".png";
	//	auto btn = ui::Button::create("res/sprites/item/box.png");
	//	btn->setEnabled(false);
	//	btnList.push_back(btn);
	//	auto item = ui::Button::create(path);
	//	item->addClickEventListener(CC_CALLBACK_1(HomeScene::itemCallback, this, i - 1));
	//	items.push_back(item);
	//}
	//layout = ui::Layout::create();
	//layout->setContentSize(Size(320, 256));
	//layout->setPosition(Vec2(visibleSize.width / 2 - layout->getContentSize().width / 2,
	//	visibleSize.height / 2 - layout->getContentSize().height / 2));
	//layout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	//layout->setBackGroundColor(Color3B::GREEN);
	//layout->setVisible(false);
	//addChild(layout,12);
	//log("layout: %f,%f", layout->getPositionX(), layout->getPositionY());

	//int max_col = layout->getContentSize().width / 64;

	//int col = 0, row = 0;
	//for (int i = 0; i < btnList.size(); i++)
	//{
	//	if (col == max_col)
	//	{
	//		row++;
	//		col = 0;
	//	}
	//	btnList[i]->setAnchorPoint(Vec2(0, 1));
	//	items[i]->setAnchorPoint(Vec2(0, 1));
	//	btnList[i]->setPosition(Vec2(col * 64, layout->getContentSize().height - 64 * row));
	//	items[i]->setPosition(btnList[i]->getPosition() + Vec2(16, -16));
	//	layout->addChild(btnList[i], 15);
	//	layout->addChild(items[i], 16);
	//	col++;
	//}

	//auto table = ui::Button::create("res/sprites/item/inventory.png");
	//table->setAnchorPoint(Point(0, 0));
	//table->setPosition(visibleSize/2);
	//
	//addChild(table, 5);

	//table->addClickEventListener(CC_CALLBACK_1(HomeScene::openInventory, this));

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