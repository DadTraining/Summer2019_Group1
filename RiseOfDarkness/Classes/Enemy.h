#ifndef ENEMY_H
#define ENEMY_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

class Enemy
{
protected:
	Sprite* mSprite, *hpBar;
	ui::LoadingBar* hpLoadingBar;
	PhysicsBody* mPhysicsBody;
	float currentHP, maxHP;
	int direction;
	float countingTime;
	int currentState;
public:
	Enemy();
	virtual ~Enemy();

	virtual void SetState(int nextState) = 0;
	virtual float GetPercentHP();
	virtual Sprite* GetSprite();
	virtual PhysicsBody* GetPhysicsBody();
	virtual void GetDamage(int damage);
	virtual bool IsAlive();
	virtual int GetDirection();
	virtual void AutoRevive(float reviveTime, float currentTime, int HP);
};

#endif