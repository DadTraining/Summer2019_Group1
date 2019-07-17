#include "Monster.h"

Monster::Monster(Layer * layer)
{
	Init();
	layer->addChild(mSprite, 1);
}

Monster::~Monster()
{
}

void Monster::SetBlood(int blood)
{
	this->mBlood = blood;
}

int Monster::GetBlood()
{
	return this->mBlood;
}

void Monster::SetDamage(int damage)
{
	this->mDamage = damage;
}

int Monster::GetDamage()
{
	return this->mDamage;
}

void Monster::SetSpeed(float speed)
{
	this->mSpeed = speed;
}

float Monster::GetSpeed()
{
	return this->mSpeed;
}

void Monster::SetRange(float range)
{
	this->mRange = range;
}

float Monster::GetRange()
{
	return this->mRange;
}

void Monster::SetDirection(int direction)
{
	this->mDirection = direction;
}

int Monster::GetDirection()
{
	return this->mDirection;
}

void Monster::SetCurrentState(int currentState)
{
	this->mCurrentState = currentState;
}

int Monster::GetCurrentState()
{
	return this->mCurrentState;
}

Sprite * Monster::GetSprite()
{
	return this->mSprite;
}

PhysicsBody * Monster::GetPhysicsBody()
{
	return this->mPhysicsBody;
}

void Monster::Init()
{
	mBlood = 100;
	mDamage = 10;
	mSpeed = 0.7f;
	mRange = 50;
	mDirection = 1;
	mCurrentState = 0;

	mSprite = Sprite::create("monster.png");
	mSprite->setScale(1.5f);

	// physic body
	mPhysicsBody = PhysicsBody::createBox(mSprite->getContentSize(), PhysicsMaterial(0, 0, 0), Vec2(0, 0));
	mPhysicsBody->setGravityEnable(false);
	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setDynamic(false);
	mPhysicsBody->setCollisionBitmask(MONSTER_BITMASK);
	mPhysicsBody->setContactTestBitmask(true);
	mSprite->setPhysicsBody(mPhysicsBody);
}

void Monster::Run()
{
	auto move1 = MoveBy::create(3.0f, Vec2(100, 0));
	auto move2 = MoveBy::create(3.0f, Vec2(-100, 0));
	
	auto delay = DelayTime::create(0.5);
	auto seq = Sequence::create(move1, delay, move2, delay, nullptr);

	auto rep = RepeatForever::create(seq);
	mSprite->runAction(rep);
}

void Monster::Update(float deltaTime)
{
	
}

bool Monster::Detect(Vec2 * pos)
{
	return false;
}
