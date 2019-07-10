#include "MainCharacter.h"

MainCharacter::MainCharacter() {};

MainCharacter::MainCharacter(Layer* layer)
{
	a = 0;
	Init();
	layer->addChild(mainCharacter, 2);
	maction[0] = RepeatForever::create(GetAnimation("res/animates/mainCharacter/goUp/goUp.plist", "%02d.png", 10));
	CC_SAFE_RETAIN(maction[0]);
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

void MainCharacter::run()
{
	mainCharacter->runAction(maction[0]);
}

Animate* MainCharacter::GetAnimation(const char *dir, const char *format, int count)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(dir);
	auto spritecache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> animFrames;
	char str[100];
	for (int i = 1; i <= count; i++)
	{
		sprintf(str, format, i);
		animFrames.pushBack(spritecache->getSpriteFrameByName(str));
	}
	auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
	return Animate::create(animation);
}

void MainCharacter::stop()
{
	mainCharacter->stopAction(maction[0]);
}