#include "Enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}

float Enemy::GetPercentHP()
{
	return currentHP / maxHP*100.0;
}

Sprite* Enemy::GetSprite()
{
	return mSprite;
}

PhysicsBody* Enemy::GetPhysicsBody()
{
	return mPhysicsBody;
}

bool Enemy::IsAlive()
{
	if (currentHP <= 0)
	{
		mPhysicsBody->setContactTestBitmask(false);
		mSprite->setVisible(false);
		hpBar->setVisible(false);
		hpLoadingBar->setVisible(false);
		return false;
	}
	return true;
}

void Enemy::GetDamage(int damage)
{
	currentHP -= damage;
}

int Enemy::GetDirection()
{
	return direction;
}

void Enemy::AutoRevive(float reviveTime, float currentTime, int HP)
{
	if (currentHP < maxHP && currentTime >= reviveTime)
	{
		currentHP += HP;
		if (currentHP > maxHP)
		{
			currentHP = maxHP;
		}
	}
}


void Enemy::SetPreventRun()
{
	switch (direction)
	{
	case 1:
		mSprite->setPositionY(mSprite->getPositionY() - speed);
		preventRun = 1;
		break;
	case 2:
		mSprite->setPositionY(mSprite->getPositionY() + speed);
		preventRun = 2;
		break;
	case 3:
		mSprite->setPositionX(mSprite->getPositionX() + speed);
		preventRun = 3;
		break;
	case 4:
		mSprite->setPositionX(mSprite->getPositionX() - speed);
		preventRun = 4;
	}
}

void Enemy::ReverseDirection()
{
	switch (direction)
	{
	case 1:
		SetDirection(2);
		break;
	case 2:
		SetDirection(1);
		break;
	case 3:
		SetDirection(4);
		break;
	case 4:
		SetDirection(3);
	}
}

void Enemy::SetDirection(int dir)
{
	if (direction != dir)
	{
		if (dir == 4)
		{
			mSprite->setFlipX(true);
		}
		else if (direction == 4)
		{
			mSprite->setFlipX(false);
		}
		direction = dir;
	}
	if (preventRun != dir)
	{
		preventRun = 0;
	}
}
