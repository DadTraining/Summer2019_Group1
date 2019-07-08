#ifndef MAIN_CHARACTER_H
#define MAIN_CHARACTER_H

#include <vector>
#include "cocos2d.h"

using namespace cocos2d;

class MainCharacter
{
private:
	int healthPoint;
	int stamina;
	int defend;
	int heartContainers;
	int speed;
	float iceResistance;
	float fireResistance;
	float poisonousResistance;
	Sprite* mainCharacter;
public:
	MainCharacter();
	MainCharacter(Scene* scene);
	~MainCharacter();
	void Init();
	Sprite* GetSprite();
	void Update(float deltaTime);
};

#endif