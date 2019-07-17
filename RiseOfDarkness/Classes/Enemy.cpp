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