#ifndef MAIN_CHARACTER_H
#define MAIN_CHARACTER_H

#include "cocos2d.h"
#include "Slash.h"
#include <vector>
#include "Arrow.h"
#include "Item.h"
#include "Inventory.h"
using namespace cocos2d;

class MainCharacter
{
public:
	// BIT MASK
	static const int MAIN_CHARACTER_BITMASK = 0;
	static const int OBSTACLE_BITMASK = 1;
	static const int SLASH_BITMASK = 2;
	static const int SPEARMOBLIN_BITMASK = 3;
	static const int PIERCE_BITMASK = 4;
	static const int NORMAL_ARROW_BITMASK = 5;

	// STATUS
	static const int SPEED = 5;
	static const int ATTACK = 20;
	static const int DEFEND = 10;

	// DAMAGE ALL CHARACTER
	static const int SPEARMOBLIN_DAMAGE = 40;
	static const int NORMAL_ARROW = 10;

	static const int FRONT_IDLE = 0;
	static const int BACK_IDLE = 1;
	static const int LEFT_IDLE = 2;
	static const int GO_DOWN = 3;
	static const int GO_UP = 4;
	static const int GO_LEFT = 5;
	static const int ROLL_FRONT = 6;
	static const int ROLL_BACK = 7;
	static const int ROLL_LEFT = 8;
	static const int FRONT_SLASH = 9;
	static const int BACK_SLASH = 10;
	static const int LEFT_SLASH = 11;
	static const int FRONT_ARCHERY = 12;
	static const int BACK_ARCHERY = 13;
	static const int LEFT_ARCHERY = 14;
	static const int FRONT_SHIELD = 15;
	static const int BACK_SHIELD = 16;
	static const int LEFT_SHIELD = 17;
	static const int FRONT_GET_DAMAGE = 18;
	static const int BACK_GET_DAMAGE = 19;
	static const int LEFT_GET_DAMAGE = 20;
	static const int DEAD = 21;


private:
	int stageLevel;
	int speed;
	int attack;
	int defend;
	std::string mName;
	float maxHP, currentHP, maxMP, currentMP;

public:
	int GetSpeed();
	int GetAttack();
	std::string GetName();
	int GetStageLevel();

public:
	static MainCharacter* GetInstance();
	void Init(std::string name);
	void Refresh();
	Sprite* GetSprite();
	PhysicsBody* GetPhysicsBody();
	int GetCurrentState();
	void Update(float deltaTime);
	void SetState(int nextStage);
	Inventory *GetInventory();
	int GetDirection();
	void SetDirection(int direction);
	void Idle();
	void NormalAttack();
	void SpecialAttack();
	void Defend();
	void Evade();
	void Run();
	void StopRun();
	void Fire(int);
	void SetListArrow(std::vector<Arrow*>);
	std::vector<Arrow*> GetListArrow();
	void StopDefend();
	float GetPercentHP();
	float GetPercentMP();
	void AutoRevive(float deltaTime);
	void AddToLayer(Layer* layer);
	void GetDamage(int damage);
	bool IsAlive();
	void SetPreventRun(int prevent);
private:
	static MainCharacter* m_instance;
	Sprite* mSprite;
	Action* mAction[22];
	PhysicsBody* mPhysicsBody;
	std::vector<Arrow*>mArrows;
	std::vector<int> mAllItems;
	std::vector<Item*> mItems;
	Inventory *inventory;
	int currentState;
	int direction;
	int preventRun;	
	float countingTime;
	Slash* slash;
	MainCharacter();
	~MainCharacter();
	void CreateMainCharacter();
};

#endif