#ifndef GAME_PLAY_H
#define GAME_PLAY_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Enemy.h"

using namespace cocos2d;

class GamePlay
{
protected:
	Sprite* mainCharacter;
	TMXTiledMap* tileMap, *upperTileMap;
	Camera* camera;
	PhysicsBody* body;
	std::vector<ui::Button*> m_buttons;
	std::vector<Sprite*> m_sprites;
	ui::Widget::TouchEventType mCurrentTouchState;
	Point mCurrentTouchPoint;
	Point mNextTouchPoint;
	Size distance;
	Label* mName, *gameover, *clear, *gold;
	ui::LoadingBar *hpLoadingBar, *mpLoadingBar;
	std::vector<Enemy*> m_enemies;
	int currentStage;
public:
	GamePlay();
	virtual ~GamePlay();
	virtual void update(float deltaTime) = 0;
	virtual void UpdateInfoBar();
	virtual void EnablePressed();
	virtual void DisablePressed();

	virtual void CreateAllButton(Layer* layer);
	virtual void CreatePhysicsWorld(const char* obstacle, const char* mc, Layer* layer);
	virtual void SetCamera(Vec2 pos);
	virtual void AddListener() = 0;
	virtual void UpdateController();

	virtual void OpenInventory(cocos2d::Ref * sender);
	virtual void ShowInventoryGrid();

	virtual bool CheckClear();
	virtual void EnemyUpdate(float deltaTime);
};

#endif
