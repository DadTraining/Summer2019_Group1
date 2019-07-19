#ifndef INPUT_NAME_SCENE_H
#define INPUT_NAME_SCENE_H

#include "cocos2d.h"
#include "UI/CocosGUI.h"

using namespace cocos2d;

class InputNameScene : public Scene
{
private:
	ui::TextField* textField;
	ui::TextField* textField2;
	
public:
	static Scene* CreateScene();

	virtual bool init();
	ui::TextField* GetTextField();

	CREATE_FUNC(InputNameScene);
};

#endif