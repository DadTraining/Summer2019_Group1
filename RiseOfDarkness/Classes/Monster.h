#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"
#include <vector>
#include <list>
#include "Bullet.h"

USING_NS_CC;
using namespace std;

class Monster
{
public:
	static const int MONSTER_BITMASK = 4;
	//static bool isRun;

private:
	int mBlood;
	int mDamage;
	float mSpeed;
	int mDirection;
	int mCurrentState;
	bool isRun;
	list<Bullet*> m_bullets;
	Sprite* mSprite;
	Action* mAction[10];
	PhysicsBody* mPhysicsBody;

public:
	Monster(Layer* layer);
	~Monster();
	void SetBlood(int blood);
	int GetBlood();
	void SetDamage(int damage);
	int GetDamage();
	void SetSpeed(float speed);
	float GetSpeed();
	void SetDirection(int direction);
	int GetDirection();
	void SetCurrentState(int currentState);
	int GetCurrentState();
	Sprite* GetSprite();
	PhysicsBody* GetPhysicsBody();

	void Init();
	void Run();
	void Update(float deltaTime);
	bool Detect(Vec2 posMc);
	void StopRun();
	void Hit();
	void StartRun();
};

#endif // !__MONSTER_H__

