#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"

USING_NS_CC;

class Bullet
{
public:
	Sprite *m_sprite;

public:
	Bullet(Layer * layer);
	~Bullet();
	Sprite* GetSprite();
	Sprite * CloneSprite(Sprite* sprite);
	void Init();
	void Update(float deltaTime, Vec2 posMt);
};

#endif // !__BULLET_H__
