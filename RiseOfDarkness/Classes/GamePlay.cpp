#include "GamePlay.h"
#include "ResourceManager.h"
#include "MainCharacter.h"

USING_NS_CC;

GamePlay::GamePlay() {}
GamePlay::~GamePlay() {}

void GamePlay::UpdateInfoBar()
{
	hpLoadingBar->setPercent(MainCharacter::GetInstance()->GetPercentHP());
	mpLoadingBar->setPercent(MainCharacter::GetInstance()->GetPercentMP());
}

void GamePlay::CreatePhysicsWorld(const char* obstacle, const char* mc, Layer* layer)
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

void GamePlay::CreateAllButton(Layer* layer)
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
	specialAttack->setAnchorPoint(Vec2(0, 1));
	layer->addChild(specialAttack, 6);
	m_buttons.push_back(specialAttack);

	// EVDAE BUTTON ID 1
	auto evade = get->GetButtonById(11);
	evade->removeFromParent();
	evade->setAnchorPoint(Vec2(0, 0));
	layer->addChild(evade, 6);
	m_buttons.push_back(evade);

	// NORMAL ATTACK BUTTON ID 2
	auto normalAttack = get->GetButtonById(12);
	normalAttack->removeFromParent();
	normalAttack->setAnchorPoint(Vec2(1, 0));
	layer->addChild(normalAttack, 6);
	m_buttons.push_back(normalAttack);

	// DEFEND BUTTON ID 3
	auto defend = get->GetButtonById(13);
	defend->removeFromParent();
	defend->setAnchorPoint(Vec2(1, 1));
	layer->addChild(defend, 6);
	m_buttons.push_back(defend);

	// PAUSE BUTTON ID 4
	auto pause = get->GetButtonById(16);
	pause->removeFromParent();
	pause->setAnchorPoint(Vec2(1, 1));
	pause->setVisible(true);
	layer->addChild(pause, 6);
	m_buttons.push_back(pause);

	// RESUME BUTTON ID 5
	auto resume = get->GetButtonById(17);
	resume->setAnchorPoint(Vec2(1, 1));
	resume->removeFromParent();
	resume->setVisible(false);
	layer->addChild(resume, 7);
	m_buttons.push_back(resume);

	// HOME BUTTON ID 6
	auto home = get->GetButtonById(24);
	home->setVisible(false);
	home->removeFromParent();
	layer->addChild(home, 12);
	m_buttons.push_back(home);

	// RETRY BUTTON ID 7
	auto retry = get->GetButtonById(23);
	retry->removeFromParent();
	retry->setVisible(false);
	layer->addChild(retry, 12);
	m_buttons.push_back(retry);

	// GAMEOVER TEXT
	gameover = get->GetLabelById(1);
	gameover->setString("GAMEOVER!");
	gameover->setVisible(false);
	gameover->removeFromParent();
	layer->addChild(gameover, 12);

	// NAME'S MAIN CHARACTER TEXT
	mName = get->GetLabelById(0);
	mName->setString(MainCharacter::GetInstance()->GetName());
	mName->removeFromParent();
	mName->setAnchorPoint(Vec2(0, 1));
	layer->addChild(mName, 12);

	// CLEAR STAGE TEXT
	clear = get->GetLabelById(2);
	clear->setString("STAGE CLEAR!");
	clear->setVisible(false);
	clear->removeFromParent();
	layer->addChild(clear, 12);

	// STATUS MC ID 8
	auto mainCharacterFace = get->GetButtonById(25);
	mainCharacterFace->setAnchorPoint(Vec2(0, 1));
	mainCharacterFace->removeFromParent();
	layer->addChild(mainCharacterFace, 8);
	m_buttons.push_back(mainCharacterFace);

	// SPRITE ID 10
	auto infoBar = get->GetSpriteById(20);
	infoBar->setAnchorPoint(Vec2(0, 1));
	infoBar->removeFromParent();
	layer->addChild(infoBar, 8);
	m_sprites.push_back(infoBar);

	//SPRITE ID 11
	auto hpBar = get->GetSpriteById(21);
	hpBar->removeFromParent();
	layer->addChild(hpBar, 9);
	m_sprites.push_back(hpBar);

	//SPRITE ID 12
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

	//GOLD FRAME SPRITE ID 13 
	auto goldFrame = get->GetSpriteById(25);
	goldFrame->setAnchorPoint(Vec2(1, 1));
	goldFrame->removeFromParent();
	layer->addChild(goldFrame, 10);
	m_sprites.push_back(goldFrame);

	// AMOUNT OF GOLD'S MAINCHARACTER
	gold = get->GetLabelById(3);
	gold->removeFromParent();
	gold->setAnchorPoint(Vec2(1, 1));
	gold->setColor(Color3B::YELLOW);
	gold->setString(std::to_string(MainCharacter::GetInstance()->GetGold()));
	layer->addChild(gold, 10);

	// HP BUTTON ID 9
	auto hpButton = get->GetButtonById(20);
	hpButton->removeFromParent();
	layer->addChild(hpButton, 11);
	m_buttons.push_back(hpButton);

	// MP BUTTON ID 10
	auto mpButton = get->GetButtonById(21);
	mpButton->removeFromParent();
	layer->addChild(mpButton, 11);
	m_buttons.push_back(mpButton);

	//BUTTON OPEN INVENTORY 11
	auto buttonOpenInventory = ui::Button::create("res/sprites/item/inventory.png");
	buttonOpenInventory->retain();
	layer->addChild(buttonOpenInventory, 11);
	m_buttons.push_back(buttonOpenInventory);

	SetCamera(mainCharacter->getPosition());
}

void GamePlay::SetCamera(Vec2 pos)
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

	// CONTROLLER
	auto frameButtonSize = m_sprites[0]->getBoundingBox().size;
	m_sprites[0]->setPosition(Vec2(pos.x - visibleSize.width / 2 + frameButtonSize.width / 2 + 10, pos.y - visibleSize.height / 2 + frameButtonSize.height / 2 + 10));
	auto frameButtonPosition = m_sprites[0]->getPosition();
	m_sprites[1]->setPosition(Vec2(frameButtonPosition.x, frameButtonPosition.y + frameButtonSize.height / 2));
	m_sprites[2]->setPosition(Vec2(frameButtonPosition.x, frameButtonPosition.y + frameButtonSize.height / 2));
	m_sprites[3]->setPosition(Vec2(frameButtonPosition.x, frameButtonPosition.y - frameButtonSize.height / 2));
	m_sprites[4]->setPosition(Vec2(frameButtonPosition.x, frameButtonPosition.y - frameButtonSize.height / 2));
	m_sprites[5]->setPosition(Vec2(frameButtonPosition.x - frameButtonSize.width / 2, frameButtonPosition.y));
	m_sprites[6]->setPosition(Vec2(frameButtonPosition.x - frameButtonSize.width / 2, frameButtonPosition.y));
	m_sprites[7]->setPosition(Vec2(frameButtonPosition.x + frameButtonSize.width / 2, frameButtonPosition.y));
	m_sprites[8]->setPosition(Vec2(frameButtonPosition.x + frameButtonSize.width / 2, frameButtonPosition.y));

	// SKILL BUTTONS
	auto frameSkillButtonSize = m_sprites[9]->getBoundingBox().size;
	m_sprites[9]->setPosition(Vec2(pos.x + visibleSize.width / 2 - frameSkillButtonSize.width / 2 - 10, pos.y - visibleSize.height / 2 + frameSkillButtonSize.height / 2 + 10));
	auto frameSkillButtonPosition = m_sprites[9]->getPosition();
	m_buttons[0]->setPosition(Vec2(frameSkillButtonPosition.x - frameSkillButtonSize.width / 3, frameSkillButtonPosition.y + frameSkillButtonSize.height / 3));
	m_buttons[1]->setPosition(Vec2(frameSkillButtonPosition.x - frameSkillButtonSize.width / 2, frameSkillButtonPosition.y - frameSkillButtonSize.height / 2));
	m_buttons[2]->setPosition(Vec2(frameSkillButtonPosition.x + frameSkillButtonSize.width / 2, frameSkillButtonPosition.y - frameSkillButtonSize.height / 2));
	m_buttons[3]->setPosition(Vec2(frameSkillButtonPosition.x + frameSkillButtonSize.width / 2, frameSkillButtonPosition.y + frameSkillButtonSize.height / 2));
	
	// PAUSE FUNCTION BUTTONS
	m_buttons[4]->setPosition(Vec2(pos + visibleSize / 2));
	m_buttons[5]->setPosition(Vec2(pos + visibleSize / 2));

	// GAMEOVER || CLEAR
	m_buttons[6]->setPosition(Vec2(pos.x - m_buttons[7]->getBoundingBox().size.width, pos.y - m_buttons[7]->getBoundingBox().size.height / 2));
	m_buttons[7]->setPosition(Vec2(pos.x + m_buttons[8]->getBoundingBox().size.width, pos.y - m_buttons[7]->getBoundingBox().size.height / 2));
	gameover->setPosition(Vec2(pos.x, pos.y + m_buttons[7]->getBoundingBox().size.height / 2));
	clear->setPosition(Vec2(pos.x, pos.y + m_buttons[7]->getBoundingBox().size.height / 2));

	m_buttons[8]->setPosition(Vec2(pos.x - visibleSize.width / 2, pos.y + visibleSize.height / 2));
	mName->setPosition(pos.x - visibleSize.width / 2 + m_buttons[8]->getBoundingBox().size.width + 10, pos.y + visibleSize.height / 2 - (m_buttons[8]->getBoundingBox().size.height / 2 - mName->getBoundingBox().size.height / 2));
	m_sprites[10]->setPosition(pos.x - visibleSize.width / 2, pos.y + visibleSize.height / 2 - m_buttons[8]->getBoundingBox().size.height);
	auto infoBarPosition = m_sprites[10]->getPosition();
	auto infoBarSize = m_sprites[10]->getBoundingBox().size;
	m_sprites[11]->setPosition(infoBarPosition.x + infoBarSize.width / 1.6, infoBarPosition.y - infoBarSize.height / 2.8);
	m_sprites[12]->setPosition(infoBarPosition.x + infoBarSize.width / 1.6, infoBarPosition.y - infoBarSize.height / 1.5);
	hpLoadingBar->setPosition(m_sprites[11]->getPosition());
	mpLoadingBar->setPosition(m_sprites[12]->getPosition());

	m_sprites[13]->setPosition(Vec2(pos.x + visibleSize.width / 3.5, pos.y + visibleSize.height / 2));
	gold->setPosition(Vec2(pos.x + visibleSize.width / 3.5, pos.y + visibleSize.height / 2 - (m_sprites[13]->getBoundingBox().size.height / 2 - gold->getBoundingBox().size.height / 2)));

	m_buttons[9]->setPosition(Vec2(pos.x + visibleSize.width / 7, pos.y - visibleSize.height / 2 + m_buttons[9]->getBoundingBox().size.height / 2));
	m_buttons[10]->setPosition(Vec2(m_buttons[9]->getPositionX() + m_buttons[10]->getBoundingBox().size.width + 5, m_buttons[9]->getPositionY()));

	m_buttons[11]->setPosition(Vec2(m_buttons[9]->getPositionX() - m_buttons[9]->getBoundingBox().size.width, m_buttons[9]->getPositionY()));
	MainCharacter::GetInstance()->GetInventory()->SetSpritePosition(Vec2(pos.x, pos.y));
}

void GamePlay::DisablePressed()
{
	m_sprites[2]->setVisible(false);
	m_sprites[4]->setVisible(false);
	m_sprites[6]->setVisible(false);
	m_sprites[8]->setVisible(false);

}

void GamePlay::EnablePressed()
{
	switch (MainCharacter::GetInstance()->GetDirection())
	{
	case 1:
		m_sprites[2]->setVisible(true);
		break;
	case 2:
		m_sprites[4]->setVisible(true);
		break;
	case 3:
		m_sprites[6]->setVisible(true);
		break;
	case 4:
		m_sprites[8]->setVisible(true);
	}
	MainCharacter::GetInstance()->Run();
}

void GamePlay::UpdateController()
{
	switch (mCurrentTouchState)
	{
	case ui::Widget::TouchEventType::BEGAN:
	case ui::Widget::TouchEventType::MOVED:
	{
		if (Rect(m_sprites[1]->getPosition().x - m_sprites[1]->getBoundingBox().size.width / 2, m_sprites[1]->getPosition().y - m_sprites[1]->getBoundingBox().size.height, m_sprites[1]->getContentSize().width, m_sprites[1]->getContentSize().height).containsPoint(mNextTouchPoint))
		{
			mainCharacter->setFlippedX(false);
			MainCharacter::GetInstance()->SetDirection(1);
			EnablePressed();
		}
		else if (Rect(m_sprites[3]->getPosition().x - m_sprites[3]->getBoundingBox().size.width / 2, m_sprites[3]->getPosition().y, m_sprites[3]->getContentSize().width, m_sprites[3]->getContentSize().height).containsPoint(mNextTouchPoint))
		{
			mainCharacter->setFlippedX(false);
			MainCharacter::GetInstance()->SetDirection(2);
			EnablePressed();
		}
		else if (Rect(m_sprites[5]->getPosition().x, m_sprites[5]->getPosition().y - m_sprites[5]->getBoundingBox().size.height / 2, m_sprites[5]->getContentSize().width, m_sprites[5]->getContentSize().height).containsPoint(mNextTouchPoint))
		{
			mainCharacter->setFlippedX(false);
			MainCharacter::GetInstance()->SetDirection(3);
			EnablePressed();
		}
		else if (Rect(m_sprites[7]->getPosition().x - m_sprites[7]->getBoundingBox().size.width, m_sprites[7]->getPosition().y - m_sprites[7]->getBoundingBox().size.height / 2, m_sprites[7]->getContentSize().width, m_sprites[7]->getContentSize().height).containsPoint(mNextTouchPoint))
		{
			mainCharacter->setFlippedX(true);
			MainCharacter::GetInstance()->SetDirection(4);
			EnablePressed();
		}
		else
		{
			DisablePressed();
			MainCharacter::GetInstance()->StopRun();
		}
		break;
	}
	case ui::Widget::TouchEventType::ENDED:
	{
		DisablePressed();
		MainCharacter::GetInstance()->StopRun();
	}
	}
}

void GamePlay::OpenInventory(cocos2d::Ref * sender)
{
	MainCharacter::GetInstance()->GetInventory()->AutoArrange();
	GamePlay::ShowInventoryGrid();
	MainCharacter::GetInstance()->GetInventory()->SetVisible(
		!(MainCharacter::GetInstance()->GetInventory()->IsVisible())
	);
}

void GamePlay::ShowInventoryGrid()
{
	auto arrows = MainCharacter::GetInstance()->GetInventory()->GetArrows();
	auto weapons = MainCharacter::GetInstance()->GetInventory()->GetItemsWeapon();
	auto items = MainCharacter::GetInstance()->GetInventory()->GetItems();
	std::vector<int> itemAmount = MainCharacter::GetInstance()->GetInventory()->GetItemAmount(0);
	std::vector<int> arrowAmount = MainCharacter::GetInstance()->GetInventory()->GetItemAmount(1);
	std::vector<Label*> amountLabel = MainCharacter::GetInstance()->GetInventory()->GetAmountLabel(0);
	std::vector<Label*> amountArrowLabel = MainCharacter::GetInstance()->GetInventory()->GetAmountLabel(1);
	int cols = 0, rows = 0;
	MainCharacter::GetInstance()->GetInventory()->GetClickBox()->removeFromParent();
	// SHOW TAB POTION
	MainCharacter::GetInstance()->GetInventory()->GetTab(1)->addChild(MainCharacter::GetInstance()->GetInventory()->GetClickBox(), 22);
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->GetIcon() != NULL)
		{
			if (cols == 6)
			{
				rows++;
				cols = 0;
			}
			MainCharacter::GetInstance()->GetInventory()->slots[i]->GetIcon()->removeFromParent();
			// get tab to add item
			MainCharacter::GetInstance()->GetInventory()->GetTab(1)->addChild(items[i]->GetIcon(), 21);
			MainCharacter::GetInstance()->GetInventory()->slots[i]->GetIcon()->setPosition(
				Vec2(64 * cols + 32,
					MainCharacter::GetInstance()->GetInventory()->GetSize().y - 64 * rows - 32) - Vec2(0, 69)
			);
			/*MainCharacter::GetInstance()->GetInventory()->slots[i]->GetIcon()->setPosition(
				Vec2(44 * cols + 22,
					MainCharacter::GetInstance()->GetInventory()->GetSize().y - 44 * rows - 50) - Vec2(-5, 25)
			);*/
			amountLabel[i]->removeFromParent();
			if (itemAmount[i]>1)
			{
				amountLabel[i]->setString(std::to_string(itemAmount[i]));
			}
			amountLabel[i]->setPosition(items[i]->GetIcon()->getPosition() + Vec2(16, -16));
			//amountLabel[i]->setPosition(items[i]->GetIcon()->getPosition() + Vec2(22, -22));
			MainCharacter::GetInstance()->GetInventory()->GetTab(1)->addChild(amountLabel[i], 22);
			cols++;
		}
	}
	//SHOW TAB WEAPON
	cols = rows = 0;
	for (int i = 0; i < weapons.size(); i++)
	{
		if (weapons[i]->GetIcon() != NULL)
		{
			if (cols == 6)
			{
				rows++;
				cols = 0;
			}
			MainCharacter::GetInstance()->GetInventory()->weapons[i]->GetIcon()->removeFromParent();
			// get tab to add item
			MainCharacter::GetInstance()->GetInventory()->GetTab(0)->addChild(weapons[i]->GetIcon(), 21);
			MainCharacter::GetInstance()->GetInventory()->weapons[i]->GetIcon()->setPosition(
				Vec2(64 * cols + 32,
					MainCharacter::GetInstance()->GetInventory()->GetSize().y - 64 * rows - 32) - Vec2(0, 69)
			);
			cols++;
		}
	}
	//SHOW TAB ARROW
	cols = rows = 0;
	MainCharacter::GetInstance()->GetInventory()->GetClickBox()->removeFromParent();
	MainCharacter::GetInstance()->GetInventory()->GetTab(3)->addChild(MainCharacter::GetInstance()->GetInventory()->GetClickBox(), 22);
	for (int i = 0; i < arrows.size(); i++)
	{
		if (arrows[i]->GetIcon() != NULL)
		{
			if (cols == 6)
			{
				rows++;
				cols = 0;
			}
			MainCharacter::GetInstance()->GetInventory()->arrows[i]->GetIcon()->removeFromParent();
			// get tab to add item
			MainCharacter::GetInstance()->GetInventory()->GetTab(3)->addChild(arrows[i]->GetIcon(), 21);
			MainCharacter::GetInstance()->GetInventory()->arrows[i]->GetIcon()->setPosition(
				Vec2(64 * cols + 32,
					MainCharacter::GetInstance()->GetInventory()->GetSize().y - 64 * rows - 32) - Vec2(0, 69)
			);
			amountArrowLabel[i]->removeFromParent();
			if (arrowAmount[i]>1)
			{
				amountArrowLabel[i]->setString(std::to_string(arrowAmount[i]));
			}
			amountArrowLabel[i]->setPosition(arrows[i]->GetIcon()->getPosition() + Vec2(16, -16));
			MainCharacter::GetInstance()->GetInventory()->GetTab(3)->addChild(amountArrowLabel[i], 22);
			cols++;
		}
	}
}

bool GamePlay::CheckClear()
{
	for (int i = 0; i < m_enemies.size(); i++)
	{
		if (m_enemies[i]->GetSprite()->isVisible())
		{
			return false;
		}
	}
	return true;
}

void GamePlay::EnemyUpdate(float deltaTime)
{
	for (int i = 0; i < m_enemies.size(); i++)
	{
		if (m_enemies[i]->GetSprite()->isVisible())
		{
			m_enemies[i]->Update(deltaTime);
		}
	}
	gold->setString(std::to_string(MainCharacter::GetInstance()->GetGold()));
}