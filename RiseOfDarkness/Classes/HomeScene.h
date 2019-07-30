#ifndef HOME_SCENE_H
#define HOME_SCENE_H

#include "MainCharacter.h"
#include "GamePlay.h"

class HomeScene : public Layer, GamePlay
{
private:
	Sprite* weaponSeller, *potionSeller, *equipmentSeller;
	Action* weapon, *potion, *equipment;
	ui::Layout* tab;
public:
	static Scene* CreateScene();
	virtual bool init();
	void InitChest();
	void update(float deltaTime);
	void AddListener();
	void ClickShowInfor(Ref* pSender);
	bool OnTouchBegan(Touch* touch, Event* event);
	bool OnTouchEnded(Touch* touch, Event* event);
	void OnTouchMoved(Touch* touch, Event* event);
	void NormalAttack(Ref* sender, ui::Widget::TouchEventType type);
	void SpecialAttack(Ref* sender, ui::Widget::TouchEventType type);
	void Evade(Ref* sender, ui::Widget::TouchEventType type);
	void Defend(Ref* sender, ui::Widget::TouchEventType type);
	bool onContactBegin(PhysicsContact& contact);
	void OpenInventory(cocos2d::Ref* sender);
	void SetCamera(Vec2 pos);
	void CreateAllButton(Layer* layer);
	void CreateNPC();
	void RunActionNPC();
	void OpenWeaponShop();
	void OpenPotionShop();
	void OpenEquipmentShop();
	void CreateShop();
	void ShowInfor();
	CREATE_FUNC(HomeScene);
};

#endif
