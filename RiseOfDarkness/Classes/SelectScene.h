
#ifndef __SELECT_SCENE_H__
#define __SELECT_SCENE_H__

#include "cocos2d.h"

class SelectScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* CreateScene();

    virtual bool init();
	void LoadLevel(cocos2d::Ref *pSender,int level);
	void CreateMap();
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    CREATE_FUNC(SelectScene);
private:
	cocos2d::TMXTiledMap *mTileMap;
	cocos2d::Sprite* player;
	cocos2d::TMXLayer *meta;
	cocos2d::TMXLayer *collectMap;
};

#endif // __HELLOWORLD_SCENE_H__
