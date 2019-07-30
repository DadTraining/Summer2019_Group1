#ifndef __LEVEL2_SCENE_H__
#define __LEVEL2_SCENE_H__

#include "GamePlay.h"
#include "cocos2d.h"
#include "Enemy.h"
using namespace cocos2d;

class Level2Scene : public Layer, GamePlay
{
public:
	static cocos2d::Scene* CreateScene();

	virtual bool init();

	void update(float deltaTime);

	void AddListener();

	void CreateMonster();

	bool OnTouchBegan(Touch* touch, Event* event);
	bool OnTouchEnded(Touch* touch, Event* event);
	void OnTouchMoved(Touch* touch, Event* event);
	void NormalAttack(Ref* sender, ui::Widget::TouchEventType type);
	void SpecialAttack(Ref* sender, ui::Widget::TouchEventType type);
	void Evade(Ref* sender, ui::Widget::TouchEventType type);
	void Defend(Ref* sender, ui::Widget::TouchEventType type);
	bool onContactBegin(PhysicsContact& contact);
	void OpenInventory(cocos2d::Ref * sender);
	void CreatePhysicsWorld(const char * obstacle, const char * mc, const char * river, Layer * layer);
	void Collision(PhysicsContact & contact, int bitmask1, int bitmask2, int type);

	CREATE_FUNC(Level2Scene);
};

#endif // !__LEVEL2_SCENE_H__

