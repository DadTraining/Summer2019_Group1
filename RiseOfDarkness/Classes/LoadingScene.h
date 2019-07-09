#pragma once
#include "cocos2d.h"

class LoadingScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* CreateScene();

	virtual bool init();
	CREATE_FUNC(LoadingScene);
};

