/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __LEVEL1SCENE_SCENE_H__
#define __LEVEL1SCENE_SCENE_H__

#include "cocos2d.h"

class Level1Scene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* CreateScene();

    virtual bool init();

	void CreateMap();

	void update(float deltaTime);
	void setViewPointCenter(cocos2d::Point position);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void setPlayerPosition(cocos2d::Point position);
	cocos2d::Point tileCoordForPosition(cocos2d::Point);
	
    CREATE_FUNC(Level1Scene);
private:
	cocos2d::TMXTiledMap *mTileMap;
	cocos2d::Sprite* player;
	cocos2d::TMXLayer *meta;
	cocos2d::TMXLayer *collectMap;
};

#endif