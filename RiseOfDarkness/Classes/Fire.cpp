#include "Fire.h"
#include "MainCharacter.h"
#include "ResourceManager.h"

USING_NS_CC;

Fire::Fire(Sprite* sprite, int BITMASK)
{
	mSprite = sprite;
	//physics body
	mPhysicsBody = PhysicsBody::createBox(Size(mSprite->getContentSize()), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setGravityEnable(false);
	mPhysicsBody->setDynamic(false);
	mPhysicsBody->setCollisionBitmask(BITMASK);
	mPhysicsBody->setContactTestBitmask(true);
	mSprite->setPhysicsBody(mPhysicsBody);
	mAction = ResourceManager::GetInstance()->GetActionById(56)->clone();

	CC_SAFE_RETAIN(mAction);
	check = true;
}

Fire::~Fire()
{
	mAction->autorelease();
}
int count = 0;
void Fire::update(float deltaTime)
{
	if (count == 4)
	{
		//StopAction();
		mSprite->setVisible(false);
		count = 0;
	}
	else
	{
		mSprite->setVisible(true);
		RunAction();
		count++;
	}
}

void Fire::SetVisible(bool b)
{
	mSprite->setVisible(b);
	if (!b)
	{
		mSprite->setPosition(Vec2(-1, -1));
	}
}

bool Fire::IsVisible()
{
	return mSprite->isVisible();
}

Sprite * Fire::GetSprite()
{
	return mSprite;
}

void Fire::SetScale(float scaleX, float scaleY)
{
	mSprite->setScale(scaleX, scaleY);
}

void Fire::AddToScene(Layer *layer)
{
	mSprite->removeFromParent();
	layer->addChild(mSprite, 7);
}

Vec2 Fire::GetPosition()
{
	return mSprite->getPosition();
}

void Fire::SetPosition(Vec2 pos)
{
	mSprite->setPosition(pos);
}

void Fire::RunAction()
{
	if (check)
	{
		mSprite->runAction(mAction);
		check = false;
	}
}

bool Fire::StopAction()
{
	/*mSprite->stopAllActions();
	check = true;*/
	return check;
}
