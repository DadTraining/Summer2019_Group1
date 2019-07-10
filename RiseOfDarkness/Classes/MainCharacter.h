#ifndef MAIN_CHARACTER_H
#define MAIN_CHARACTER_H

#include <vector>
#include "cocos2d.h"

using namespace cocos2d;

class MainCharacter
{
private:
	float a;
	int healthPoint;
	int stamina;
	int defend;
	int heartContainers;
	int speed;
	float iceResistance;
	float fireResistance;
	float poisonousResistance;
	Sprite* mainCharacter;
	Action* maction[10];
public:
	MainCharacter();
	MainCharacter(Layer* layer);
	~MainCharacter();
	void Init();
	Sprite* GetSprite();
	void Update(float deltaTime);
	void run();
	void stop();
	Animate* GetAnimation(const char * dir, const char *format, int count);

};

#endif