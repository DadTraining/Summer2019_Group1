#ifndef _FIRE_H_
#define _FIRE_H_

#include "cocos2d.h"

using namespace cocos2d;

class Fire
{
private:
	bool check;
	Sprite *mSprite;
	PhysicsBody* mPhysicsBody;
	Action* mAction;
public:
	Fire(Sprite *sprite, int BITMASK);
	~Fire();
	void update(float);
	void SetVisible(bool);
	bool IsVisible();
	Sprite* GetSprite();
	void SetScale(float, float);
	void AddToScene(cocos2d::Layer*);
	Vec2 GetPosition();
	void SetPosition(cocos2d::Vec2 pos);
	void RunAction();
	bool StopAction();
};
#endif // !_FIRE_H_