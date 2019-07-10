#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
USING_NS_CC;

class ResourceManager
{
private:
	static ResourceManager* m_instance;
	std::string m_dataFolderPath;
	std::map<int, Sprite*> m_sprites;
	std::map<int, ui::Button*>m_buttons;
	std::map<int, Label*>m_labels;
	std::map<int, Animate*> m_animates;
	std::map<int, TMXTiledMap*> m_tileMap;
	ResourceManager();
	~ResourceManager();
public:
	static ResourceManager* GetInstance();
	void Init(const std::string &path);
	Sprite* GetSpriteById(int id);
	ui::Button* GetButtonById(int id);
	Label* GetLabelById(int id);
	Animate* GetAnimateById(int id);
	TMXTiledMap* GetTileMapById(int id);
	Sprite* DuplicateSprite(Sprite* sprite);
	Animate* GetAnimate(std::string &dir, std::string &format, int count);
};

#endif