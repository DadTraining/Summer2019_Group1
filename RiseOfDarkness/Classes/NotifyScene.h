#pragma once
#ifndef NOTIFY_SCENE_H
#define NOTIFY_SCENE_H

#include "cocos2d.h"
#include "UI/CocosGUI.h"

using namespace cocos2d;

class NotifyScene : public Scene
{

public:
	static Scene* CreateScene();

	virtual bool init();
	CREATE_FUNC(NotifyScene);
};

#endif