#ifndef __MAOKAI_H__
#define __MAOKAI_H__

#include "Enemy.h"
#include "cocos2d.h"
#include "Bullet.h"

class Maokai : public Enemy
{
private:
	static const int IDLE = 0;
	static const int ATTACK = 1;

	static const int HP = 300;
	static const int DETECT_RANGE = 200;
	static const int ATTACK_RANGE = 200;
	static const int ATTACK_COOLDOWN = 3;
	static const int REVIVE_TIME = 4;
	static const int HP_REVIVE = 10;

	Action* mAction[2];
	Bullet* bullet;
public:
	Maokai();
	Maokai(Layer* layer, Vec2 pos, int group);
	~Maokai();

	void Update(float deltaTime);

	void SetState(int nextState);

	void Idle();

	void Attack();

	Bullet* GetBullet();

	bool Detect(float detectRange);
};

#endif // !__MAOKAI_H__
