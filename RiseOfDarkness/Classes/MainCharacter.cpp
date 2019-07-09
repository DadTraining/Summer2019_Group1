#include "MainCharacter.h"

MainCharacter::MainCharacter() {};

MainCharacter::MainCharacter(Layer* layer)
{
	Init();
	layer->addChild(mainCharacter, 2);
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
	mainCharacter->setScale(2);
}

void MainCharacter::Update(float deltaTime)
{

}

Sprite* MainCharacter::GetSprite()
{
	return mainCharacter;
}