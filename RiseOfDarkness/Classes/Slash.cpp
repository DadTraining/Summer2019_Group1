#include "Slash.h"
#include "MainCharacter.h"
#include "ResourceManager.h"

Slash::Slash(Layer* layer)
{
	mSprite = ResourceManager::GetInstance()->GetSpriteById(23);
	mSprite->setVisible(false);
	mPhysicsBody = PhysicsBody::createBox(mSprite->getContentSize(), PhysicsMaterial(0, 0, 0));
	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setCollisionBitmask(MainCharacter::SLASH_BITMASK);
	mPhysicsBody->setDynamic(false);
	mPhysicsBody->setGravityEnable(false);
	mSprite->setPhysicsBody(mPhysicsBody);
	layer->addChild(mSprite, 1);
}

Slash::~Slash() {}

Sprite* Slash::GetSprite()
{
	return mSprite;
}

PhysicsBody* Slash::GetPhysicsBody()
{
	return mPhysicsBody;
}