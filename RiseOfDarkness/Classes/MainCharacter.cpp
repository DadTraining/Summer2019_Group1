#include "MainCharacter.h"
#include "ResourceManager.h"

MainCharacter* MainCharacter::m_instance = nullptr;

MainCharacter::MainCharacter() {}

MainCharacter* MainCharacter::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new MainCharacter();
	}
	return m_instance;
}

MainCharacter::~MainCharacter() {}

void MainCharacter::Init()
{
	CreateMainCharacter();
	currentState = FRONT_IDLE;
	previousState = FRONT_IDLE;
	SetState(FRONT_IDLE);
	stageLevel = 1;
	CreatePhysicsBody();
}

void MainCharacter::CreateMainCharacter()
{
	mainCharacter = ResourceManager::GetInstance()->GetSpriteById(7);
	mainCharacter->setScale(2.0);
	mAction[FRONT_IDLE] = ResourceManager::GetInstance()->GetActionById(3);
	mAction[BACK_IDLE] = ResourceManager::GetInstance()->GetActionById(4);
	mAction[LEFT_IDLE] = ResourceManager::GetInstance()->GetActionById(5);
	mAction[GO_DOWN] = ResourceManager::GetInstance()->GetActionById(1);
	mAction[GO_UP] = ResourceManager::GetInstance()->GetActionById(0);
	mAction[GO_LEFT] = ResourceManager::GetInstance()->GetActionById(2);
	mAction[ROLL_FRONT] = ResourceManager::GetInstance()->GetActionById(15);
	mAction[ROLL_BACK] = ResourceManager::GetInstance()->GetActionById(16);
	mAction[ROLL_LEFT] = ResourceManager::GetInstance()->GetActionById(17);
	mAction[FRONT_SLASH] = ResourceManager::GetInstance()->GetActionById(6);
	mAction[BACK_SLASH] = ResourceManager::GetInstance()->GetActionById(7);
	mAction[LEFT_SLASH] = ResourceManager::GetInstance()->GetActionById(8);
	mAction[FRONT_ARCHERY] = ResourceManager::GetInstance()->GetActionById(9);
	mAction[BACK_ARCHERY] = ResourceManager::GetInstance()->GetActionById(10);
	mAction[LEFT_ARCHERY] = ResourceManager::GetInstance()->GetActionById(11);
	mAction[FRONT_SHIELD] = ResourceManager::GetInstance()->GetActionById(12);
	mAction[BACK_SHIELD] = ResourceManager::GetInstance()->GetActionById(13);
	mAction[LEFT_SHIELD] = ResourceManager::GetInstance()->GetActionById(14);
}

void MainCharacter::CreatePhysicsBody()
{
	mPhysicsBody = PhysicsBody::createBox(mainCharacter->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
	mPhysicsBody->setDynamic(true);
	mPhysicsBody->setGravityEnable(false);
	mPhysicsBody->setRotationEnable(false);
	mainCharacter->setPhysicsBody(mPhysicsBody);
}

void MainCharacter::AddToScene(Layer* layer)
{
	
	mainCharacter->removeFromParent();
	layer->addChild(mainCharacter,1);
}

void MainCharacter::DoAction()
{
	mainCharacter->stopAction(mAction[previousState]);
	mainCharacter->runAction(mAction[currentState]);
}

Sprite* MainCharacter::GetSprite()
{
	return mainCharacter;
}

void MainCharacter::SetState(int state)
{
	previousState = currentState;
	currentState = state;
	DoAction();
}

int MainCharacter::GetCurrentState()
{
	return currentState;
}

void MainCharacter::Update(float deltaTime)
{

}

PhysicsBody* MainCharacter::GetPhysicsBody()
{
	return mPhysicsBody;
}