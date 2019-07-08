#ifndef HOME_SCENE_H
#define HOEM_SCENE_H

#include "MainCharacter.h"
#include "ui/CocosGUI.h"

class HomeScene : public Scene
{
private:
	MainCharacter* mainCharacter;
	TMXTiledMap* tileMap;
	ui::Button *upButton, *downButton, *rightButton, *leftButton;
	std::vector<int> heldButtons;
public:
	static Scene* CreateScene();
	virtual bool init();
	void update(float deltaTime);
	void AddListener();
	bool OnTouchBegan(Touch* touch, Event* event);
	bool OnTouchEnded(Touch* touch, Event* event);
	void OnTouchMoved(Touch* touch, Event* event);
	void UpButtonTouched(Ref* sender, ui::Widget::TouchEventType type);
	void DownButtonTouched(Ref* sender, ui::Widget::TouchEventType type);
	void LeftButtonTouched(Ref* sender, ui::Widget::TouchEventType type);
	void RightButtonTouched(Ref* sender, ui::Widget::TouchEventType type);
	void OnButtonHold(float interval);
	
	CREATE_FUNC(HomeScene);
};

#endif
