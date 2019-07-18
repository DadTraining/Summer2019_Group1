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

	// PAUSE BUTTON ID 4
	auto pause = get->GetButtonById(16);
	pause->removeFromParent();
	pause->setAnchorPoint(Vec2(1, 1));
	layer->addChild(pause, 6);
	m_buttons.push_back(pause);

	// RESUME BUTTON ID 5
	auto resume = get->GetButtonById(17);
	resume->removeFromParent();
	resume->setVisible(false);
	layer->addChild(resume, 6);
	m_buttons.push_back(resume);

	// RETURN MAP BUTTON ID 6
	auto returnMap = get->GetButtonById(18);
	returnMap->removeFromParent();
	returnMap->setVisible(false);
	layer->addChild(returnMap, 6);
	m_buttons.push_back(returnMap);

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
	layer->addChild(hpLoadingBar, 10);

	mpLoadingBar = get->GetLoadingbar(2);
	mpLoadingBar->removeFromParent();
	layer->addChild(mpLoadingBar, 10);

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
	
	m_buttons[4]->setPosition(Vec2(pos + visibleSize / 2));
	m_buttons[5]->setPosition(Vec2(pos.x, pos.y + m_buttons[5]->getBoundingBox().size.height));
	m_buttons[6]->setPosition(Vec2(pos.x, pos.y - m_buttons[6]->getBoundingBox().size.height));

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