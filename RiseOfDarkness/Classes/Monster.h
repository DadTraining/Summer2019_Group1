#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"
#include <vector>
#include <list>
#include "Bullet.h"
#include "Enemy.h"

USING_NS_CC;
using namespace std;

class Monster : public Enemy
{
public:
	static const int MONSTER_BITMASK = 10;

	static const int UP = 0;
	static const int DOWN = 1;
	static const int LEFT = 2;
	static const int RIGHT = 3;

private:
	int mBlood;
	int mDamage;
	float mSpeed;
	int mDirMove;
	int mDirHit;
	int mCurrentState;
	bool isRun;
	list<Bullet*> m_bullets;
	Sprite* mSprite;
	Action* mAction[10];
	PhysicsBody* mPhysicsBody;

public:
	Monster(Layer* layer, int id);
	~Monster();
	void SetBlood(int blood);
	int GetBlood();
	void SetDamage(int damage);
	int GetDamage();
	void SetSpeed(float speed);
	float GetSpeed();
	void SetDirMove(int direction);
	int GetDirMove();
	void SetDirHit(int direction);
	int GetDirhit();
	void SetCurrentState(int currentState);
	int GetCurrentState();
	Sprite* GetSprite();
	PhysicsBody* GetPhysicsBody();

	void Init();
	void Run();
	void MoveHit(Vec2 posMc);
	void Update(float deltaTime);
	bool Detect(Vec2 posMc);
	void StopRun();
	void Hit();
	void ResumRun();
};

#endif // !__MONSTER_H__

