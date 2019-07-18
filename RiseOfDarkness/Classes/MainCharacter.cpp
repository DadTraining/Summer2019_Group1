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

void MainCharacter::AddToLayer(Layer* layer)
{
	layer->addChild(mSprite, 1);
	slash = new Slash(layer);
}

void MainCharacter::Init(std::string name)
{
	mName = name;
	CreateMainCharacter();
}

void MainCharacter::CreateMainCharacter()
{
	auto get = ResourceManager::GetInstance();

	// CREATE SPRITE
	mSprite = get->GetSpriteById(0);
	mSprite->removeFromParent();
	mSprite->setScale(2.0);

	// CREATE ACTION
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
	mAction[FRONT_GET_DAMAGE] = ResourceManager::GetInstance()->GetActionById(19);
	mAction[BACK_GET_DAMAGE] = ResourceManager::GetInstance()->GetActionById(18);
	mAction[LEFT_GET_DAMAGE] = ResourceManager::GetInstance()->GetActionById(20);
	mAction[DEAD] = ResourceManager::GetInstance()->GetActionById(21);

	// CREATE PHYSICS BODY 
	box.width = mSprite->getContentSize().width / 1.5;
	box.height = mSprite->getContentSize().height / 3;
	mPhysicsBody = PhysicsBody::createBox(box, PhysicsMaterial(0, 0, 0), Vec2(0, -box.height));
	mPhysicsBody->setGravityEnable(false);
	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setDynamic(false);
	mPhysicsBody->setCollisionBitmask(MAIN_CHARACTER_BITMASK);
	mPhysicsBody->setContactTestBitmask(true);
	mSprite->setPhysicsBody(mPhysicsBody);

	// CREATE ALL STATUS
	stageLevel = 1;
	direction = 2;
	currentState = FRONT_IDLE;
	speed = SPEED;
	attack = ATTACK;
	defend = DEFEND;
	countingTime = 0;
	maxHP = 200;
	maxMP = 100;
	currentHP = maxHP;
	currentMP = maxMP;
}

void MainCharacter::Refresh()
{
	direction = 2;
	currentState = FRONT_IDLE;
	currentHP = maxHP;
	currentMP = maxMP;
}

void MainCharacter::SetState(int nextState)
{
	if (currentState != DEAD)
	{
		if (currentState == nextState && mSprite->getNumberOfRunningActions() == 0)
		{
			mSprite->runAction(mAction[nextState]);
		}
		else if (currentState == nextState)
		{
			return;
		}
		else
		{
			switch (nextState)
			{
			case BACK_IDLE:
			case FRONT_IDLE:
			case LEFT_IDLE:
				if (currentState == FRONT_IDLE || currentState == LEFT_IDLE || currentState == BACK_IDLE || mSprite->getNumberOfRunningActions() == 0)
				{
					mSprite->stopAllActions();
					mSprite->runAction(mAction[nextState]);
					currentState = nextState;
				}
				break;
			case GO_UP:
			case GO_DOWN:
			case GO_LEFT:
				if ((currentState == FRONT_IDLE || currentState == BACK_IDLE || currentState == LEFT_IDLE))
				{
					mSprite->stopAllActions();
					mSprite->runAction(mAction[nextState]);
					currentState = nextState;
				}
				break;
			case FRONT_SLASH:
			case BACK_SLASH:
			case LEFT_SLASH:
				currentState = nextState;
				mSprite->stopAllActions();
				mSprite->runAction(mAction[nextState]);
				break;
			case ROLL_BACK:
			case ROLL_FRONT:
			case ROLL_LEFT:
				currentState = nextState;
				mSprite->stopAllActions();
				mSprite->runAction(mAction[nextState]);
				break;
			case FRONT_SHIELD:
			case BACK_SHIELD:
			case LEFT_SHIELD:
				currentState = nextState;
				mSprite->stopAllActions();
				mSprite->runAction(mAction[nextState]);
				break;
			case FRONT_ARCHERY:
			case BACK_ARCHERY:
			case LEFT_ARCHERY:
				currentState = nextState;
				mSprite->stopAllActions();
				mSprite->runAction(mAction[nextState]);
				break;
			case FRONT_GET_DAMAGE:
			case BACK_GET_DAMAGE:
			case LEFT_GET_DAMAGE:
				currentState = nextState;
				mSprite->stopAllActions();
				mSprite->runAction(mAction[nextState]);
				break;
			case DEAD:
				currentState = nextState;
				mSprite->stopAllActions();
				mSprite->runAction(mAction[nextState]);
				break;
			}
		}
	}
}

void MainCharacter::Update(float deltaTime)
{
	if (IsAlive())
	{
		Idle();

		AutoRevive(deltaTime);
	}
}

void MainCharacter::Idle()
{
	if (currentState != FRONT_SHIELD && currentState != BACK_SHIELD && currentState != LEFT_SHIELD && mSprite->getNumberOfRunningActions() == 0)
	{
		if (currentState == BACK_SLASH || currentState == FRONT_SLASH || currentState == LEFT_SLASH)
		{
			slash->GetPhysicsBody()->setContactTestBitmask(false);
		}
		switch (direction)
		{
		case 1:
			SetState(BACK_IDLE);
			break;
		case 2:
			SetState(FRONT_IDLE);
			break;
		default:
			SetState(LEFT_IDLE);
		}
	}
}

void MainCharacter::Defend()
{
	if (currentState == GO_UP || currentState == GO_DOWN || currentState == GO_LEFT || currentState == FRONT_IDLE || currentState == LEFT_IDLE || currentState == BACK_IDLE)
	{
		switch (direction)
		{
		case 1:
			SetState(BACK_SHIELD);
			break;
		case 2:
			SetState(FRONT_SHIELD);
			break;
		default:
			SetState(LEFT_SHIELD);
		}
	}
}

void MainCharacter::StopDefend()
{
	switch (direction)
	{
	case 1:
		SetState(BACK_IDLE);
		break;
	case 2:
		SetState(FRONT_IDLE);
		break;
	default:
		SetState(LEFT_IDLE);
	}
}

void MainCharacter::SpecialAttack()
{
	if (currentState == GO_UP || currentState == GO_DOWN || currentState == GO_LEFT || currentState == FRONT_IDLE || currentState == LEFT_IDLE || currentState == BACK_IDLE)
	{
		switch (direction)
		{
		case 1:
			SetState(BACK_ARCHERY);
			break;
		case 2:
			SetState(FRONT_ARCHERY);
			break;
		default:
			SetState(LEFT_ARCHERY);
		}
	}
}

void MainCharacter::NormalAttack()
{
	if (currentState == GO_UP || currentState == GO_DOWN || currentState == GO_LEFT || currentState == FRONT_IDLE || currentState == LEFT_IDLE || currentState == BACK_IDLE)
	{
		slash->GetPhysicsBody()->setContactTestBitmask(true);
		switch (direction)
		{
		case 1:
			SetState(BACK_SLASH);
			slash->GetSprite()->setPosition(Vec2(mSprite->getPositionX(), mSprite->getPositionY() + 20));
			slash->GetSprite()->setRotation(0);
			break;
		case 2:
			SetState(FRONT_SLASH);
			slash->GetSprite()->setPosition(Vec2(mSprite->getPositionX(), mSprite->getPositionY() - 20));
			slash->GetSprite()->setRotation(0);
			break;
		case 3:
			SetState(LEFT_SLASH);
			slash->GetSprite()->setPosition(Vec2(mSprite->getPositionX() - 25, mSprite->getPositionY()));
			slash->GetSprite()->setRotation(90);
			break;
		case 4:
			SetState(LEFT_SLASH);
			slash->GetSprite()->setPosition(Vec2(mSprite->getPositionX() + 25, mSprite->getPositionY()));
			slash->GetSprite()->setRotation(90);
			break;
		}
	}
}

void MainCharacter::Evade()
{
	if ((currentState == GO_UP || currentState == GO_DOWN || currentState == GO_LEFT || currentState == FRONT_IDLE || currentState == LEFT_IDLE || currentState == BACK_IDLE)
		&& currentMP >= 30)
	{
		switch (direction)
		{
		case 1:
			SetState(ROLL_BACK);
			mSprite->runAction(MoveBy::create(0.9f, Vec2(0, Director::getInstance()->getVisibleSize().width / 12)));
			break;
		case 2:
			SetState(ROLL_FRONT);
			mSprite->runAction(MoveBy::create(0.9f, Vec2(0, -Director::getInstance()->getVisibleSize().width / 12)));
			break;
		case 3:
			SetState(ROLL_LEFT);
			mSprite->runAction(MoveBy::create(1.0f, Vec2(-Director::getInstance()->getVisibleSize().width / 12, 0)));
			break;
		case 4:
			SetState(ROLL_LEFT);
			mSprite->runAction(MoveBy::create(1.0f, Vec2(Director::getInstance()->getVisibleSize().width / 12, 0)));
		}
		currentMP -= 30;
	}
}

void MainCharacter::Run()
{
	if (currentState != FRONT_SHIELD && currentState != BACK_SHIELD && currentState != LEFT_SHIELD && currentHP > 0)
	{
		switch (direction)
		{
		case 1:
			SetState(GO_UP);
			mSprite->setPositionY(mSprite->getPositionY() + speed);
			break;
		case 2:
			SetState(GO_DOWN);
			mSprite->setPositionY(mSprite->getPositionY() - speed);
			break;
		case 3:
			SetState(GO_LEFT);
			mSprite->setPositionX(mSprite->getPositionX() - speed);
			break;
		case 4:
			SetState(GO_LEFT);
			mSprite->setPositionX(mSprite->getPositionX() + speed);
		}
	}
}

void MainCharacter::GetDamage(int damage)
{
	currentHP += defend - damage;
	slash->GetPhysicsBody()->setContactTestBitmask(false);
	switch (direction)
	{
	case 1:
		SetState(BACK_GET_DAMAGE);
		break;
	case 2:
		SetState(FRONT_GET_DAMAGE);
		break;
	default:
		SetState(LEFT_GET_DAMAGE);
	}
}

void MainCharacter::StopRun()
{
	mSprite->stopAction(mAction[GO_UP]);
	mSprite->stopAction(mAction[GO_DOWN]);
	mSprite->stopAction(mAction[GO_LEFT]);
}

bool MainCharacter::IsAlive()
{
	if (currentHP <= 0)
	{
		SetState(DEAD);
		return false;
	}
	return true;
}

void MainCharacter::AutoRevive(float deltaTime)
{
	if (currentMP < maxMP)
	{
		countingTime += deltaTime;
		if (countingTime >= 0.5)
		{
			countingTime = 0;
			currentMP += 5;
			if (currentMP > maxMP)
			{
				currentMP = maxMP;
			}
		}
	}
}

float MainCharacter::GetPercentHP()
{
	return currentHP / maxHP * 100.0;
}

float MainCharacter::GetPercentMP()
{
	return currentMP / maxMP * 100.0;
}

PhysicsBody* MainCharacter::GetPhysicsBody()
{
	return mPhysicsBody;
}

int MainCharacter::GetSpeed()
{
	return speed;
}

int MainCharacter::GetCurrentState()
{
	return currentState;
}

Sprite* MainCharacter::GetSprite()
{
	return mSprite;
}

std::string MainCharacter::GetName()
{
	return mName;
}

int MainCharacter::GetStageLevel()
{
	return stageLevel;
}

int MainCharacter::GetDirection()
{
	return direction;
}

void MainCharacter::SetDirection(int direction)
{
	this->direction = direction;
}