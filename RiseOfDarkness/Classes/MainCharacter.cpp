#include "MainCharacter.h"

USING_NS_CC;

MainCharacter::MainCharacter() {};

MainCharacter::MainCharacter(Scene* scene)
{
	Init();
	scene->addChild(mainCharacter);
}

MainCharacter::~MainCharacter() {};

void MainCharacter::Init()
{
	healthPoint = 500;
	stamina = 100;
	defend = 20;
	heartContainers = 0;
	speed = 1;
	iceResistance = 0;
	fireResistance = 0;
	poisonousResistance = 0;
	mainCharacter = Sprite::create("res/sprites/mainCharacter.png");
}

void MainCharacter::Update(float deltaTime)
{

}

Sprite* MainCharacter::GetSprite()
{
	return mainCharacter;
}