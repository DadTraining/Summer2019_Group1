#pragma once

#ifndef GAMEOVER_SCENE_H
#define GAMEOVER_SCENE_H

#include "cocos2d.h"
#include "UI/CocosGUI.h"

using namespace cocos2d;

class GameOverScene : public Scene
{

public:
	static Scene* CreateScene();

	virtual bool init();
	CREATE_FUNC(GameOverScene);
};

#endif