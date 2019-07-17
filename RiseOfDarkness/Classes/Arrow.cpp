#include "Arrow.h"
#include "MainCharacter.h"
USING_NS_CC;

Arrow::Arrow(Sprite* sprite)
{
	mSprite = sprite; 
	//physics body
	mPhysicsBody = PhysicsBody::createBox(Size(mSprite->getContentSize()), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setGravityEnable(false);
	mPhysicsBody->setCollisionBitmask(102);
	mPhysicsBody->setContactTestBitmask(true);	
	mSprite->setPhysicsBody(mPhysicsBody);
	setAlive(false);
}

Arrow::~Arrow()
{
}

void Arrow::setAlive(bool alive)
{
	mIsAlive = alive;
}

void Arrow::fly(bool isRight)
{
	
}

bool Arrow::isAlive()
{
	return mIsAlive;
}


void Arrow::update(float deltaTime)
{
	SetDistance(GetDistance() + RUN_STEP);
	int direction = this->GetDirection();
	if (direction==LEFT)
	{
		SetPosition(GetPosition() + Vec2(-RUN_STEP, 0));
	}
	else
	{
		if (direction==UP)
		{
			//SetRotate(90);
			SetPosition(GetPosition() + Vec2(0, RUN_STEP));
		}
		else
		{
			if (direction==DOWN)
			{
				//SetRotate(360 - 90);
				SetPosition(GetPosition() + Vec2(0, -RUN_STEP));
			}
			else
			{
				//SetFlipX(true);
				SetPosition(GetPosition() + Vec2(+RUN_STEP, 0));
			}
		}
	}

		//out of range
		if (GetDistance()>=RANGE)
		{
			//setAlive(false);
			mSprite->setVisible(false);
			SetDistance(0);
		}
}

void Arrow::SetVisible(bool b)
{
	mSprite->setVisible(b);
}

bool Arrow::IsVisible()
{
	return mSprite->isVisible();
}

void Arrow::SetRotate(int angle)
{
	mSprite->setRotation(angle);
}

void Arrow::SetFlipX(bool b)
{
	mSprite->setFlipX(b);
}

void Arrow::SetDirection(int d)
{
	direction = d;
	switch (d)
	{
	case UP:
		SetRotate(90);
		break;
	case DOWN:
		SetRotate(360 - 90);
		break;
	case RIGHT:
		SetRotate(180);
		break;
	default:
		SetRotate(0);
		break;
	}
}

void Arrow::SetDistance(int s)
{
	distance = s;
}

int Arrow::GetDistance()
{
	return distance;
}

int Arrow::GetDirection()
{
	return direction;
}

cocos2d::Sprite * Arrow::GetSprite()
{
	return mSprite;
}

void Arrow::SetScale(float scaleX,float scaleY)
{
	mSprite->setScale(scaleX,scaleY);
}

void Arrow::AddToScene(Layer *layer)
{
	layer->addChild(mSprite);
}

cocos2d::Vec2 Arrow::GetPosition()
{
	return mSprite->getPosition();
}

void Arrow::SetPosition(Vec2 pos)
{
	mSprite->setPosition(pos);
}
