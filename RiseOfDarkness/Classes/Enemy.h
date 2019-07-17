#ifndef ENEMY_H
#define ENEMY_H

#include "cocos2d.h"

using namespace cocos2d;

class Enemy
{
protected:
	Sprite* mSprite;
	PhysicsBody* mPhysicsBody;
	float currentHP, maxHP;
	int direction;
	float countingTime;
public:
	Enemy();
	virtual ~Enemy();

	virtual float GetPercentHP();
	virtual Sprite* GetSprite();
	virtual PhysicsBody* GetPhysicsBody();
	virtual void GetDamage(int damage);
	virtual bool IsAlive();
	virtual int GetDirection();
	virtual void AutoRevive(float reviveTime, float currentTime, int HP);
};

#endif