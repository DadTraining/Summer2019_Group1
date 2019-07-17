#ifndef MAIN_CHARACTER_H
#define MAIN_CHARACTER_H

#include <vector>
#include "cocos2d.h"
#include "Arrow.h"
using namespace cocos2d;

class MainCharacter
{
public:
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

public:
	static MainCharacter* GetInstance();
	void Init(std::string name);
	void Refresh();
	void AddToScene(Layer* layer);
	Sprite* GetSprite();
	PhysicsBody* GetPhysicsBody();
	int GetCurrentState();
	void Update(float deltaTime);
	void SetState(int nextStage);
	int GetSpeed();
	std::string GetName();
	int GetStageLevel();
	int GetDirection();
	void SetDirection(int direction);
	void Idle();
	void NormalAttack();
	void SpecialAttack();
	void Defend();
	void Evade();
	void Run();
	void StopRun();
	void StopDefend();
	void Fire(int);
	void SetListArrow(std::vector<Arrow*>);
	//get list arrows
	std::vector<Arrow*> GetListArrow();
private:
	static MainCharacter* m_instance;
	Sprite* mSprite;
	Action* mAction[18];
	PhysicsBody* mPhysicsBody;
	std::vector<Arrow*>mArrows;
	int stageLevel;
	int currentState;
	int direction;
	int speed;
	int preventRun;
	int maxHP, currentHP;
	int maxMP, currentMP;
	float countingTime;
	std::string mName;
	MainCharacter();
	~MainCharacter();
	void CreateMainCharacter();
};

#endif