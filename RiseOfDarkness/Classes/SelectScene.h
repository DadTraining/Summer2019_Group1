#pragma once

#include "cocos2d.h"

class SelectScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* CreateScene();

	virtual bool init();

	void GoToMapScene(cocos2d::Ref*);
	CREATE_FUNC(SelectScene);
};

