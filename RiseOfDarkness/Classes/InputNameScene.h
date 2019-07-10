#pragma once
#include "cocos2d.h"

class InputNameScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* CreateScene();

	virtual bool init();

	CREATE_FUNC(InputNameScene);
};
