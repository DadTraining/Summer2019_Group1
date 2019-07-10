#include "ResourceManager.h"
#include <iostream>

ResourceManager * ResourceManager::m_instance = nullptr;

ResourceManager::ResourceManager() {}

ResourceManager* ResourceManager::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new ResourceManager();
	}

	return m_instance;
}

ResourceManager::~ResourceManager()
{
	for (int i = 0; i < m_sprites.size(); i++)
	{
		m_sprites[i]->autorelease();
	}

	for (int i = 0; i < m_buttons.size(); i++)
	{
		m_sprites[i]->autorelease();
	}

	for (int i = 0; i < m_labels.size(); i++)
	{
		m_sprites[i]->autorelease();
	}

	for (int i = 0; i < m_animates.size(); i++)
	{
		m_sprites[i]->autorelease();
	}
}

void ResourceManager::Init(const std::string &path)
{
	if (FileUtils::getInstance()->isFileExist(path))
	{
		std::string data, tempStr1, tempStr2;
		int amount;
		int count;
		data = FileUtils::getInstance()->getStringFromFile(path);
		std::istringstream istr(data);

		// Sprite
		istr >> tempStr1;
		amount = std::stoi(tempStr1);
		for (int i = 0; i < amount; i++)
		{
			istr >> tempStr1 >> tempStr1;
			m_sprites.insert(std::pair<int, Sprite*>(i, Sprite::create(tempStr1)));
			m_sprites[i]->retain();
		}

		// Button
		istr >> tempStr1;
		amount = std::stoi(tempStr1);
		for (int i = 0; i < amount; i++)
		{
			istr >> tempStr1 >> tempStr1;
			istr >> tempStr2;
			m_buttons.insert(std::pair<int, ui::Button*>(i, ui::Button::create(tempStr1, tempStr2)));
			m_buttons[i]->retain();
		}

		// Animate
		istr >> tempStr1;
		amount = std::stoi(tempStr1);
		for (int i = 0; i < amount; i++)
		{
			istr >> tempStr1 >> tempStr1;
			count = std::stoi(tempStr1);
			istr >> tempStr1;
			istr >> tempStr2;
			m_animates.insert(std::pair<int, Animate*>(i, GetAnimate(tempStr2, tempStr1, count)));
			m_animates[i]->retain();
		}

		// Tile Map
		istr >> tempStr1;
		amount = std::stoi(tempStr1);
		for (int i = 0; i < amount; i++)
		{
			istr >> tempStr1 >> tempStr1;
			m_tileMap.insert(std::pair<int, TMXTiledMap*>(i, TMXTiledMap::create(tempStr1)));
			m_tileMap[i]->retain();
		}

		// Label
		istr >> tempStr1;
		amount = std::stoi(tempStr1);
		for (int i = 0; i < amount; i++)
		{
			istr >> tempStr1 >> tempStr1;
			m_labels.insert(std::pair<int, Label*>(i, Label::createWithTTF("", tempStr1, 30)));
			m_labels[i]->retain();
		}
	}
}

Sprite* ResourceManager::GetSpriteById(int id)
{
	return m_sprites[id];
}

ui::Button* ResourceManager::GetButtonById(int id)
{
	return m_buttons[id];
}

Label* ResourceManager::GetLabelById(int id)
{
	return m_labels[id];
}

Animate* ResourceManager::GetAnimateById(int id)
{
	return m_animates[id];
}

TMXTiledMap* ResourceManager::GetTileMapById(int id)
{
	return m_tileMap[id];
}

Sprite* ResourceManager::DuplicateSprite(Sprite* sprite)
{
	Sprite* pRet = Sprite::createWithTexture(sprite->getTexture());
	if (sprite->getChildrenCount() > 0)
	{
		for (int child = 0; child < sprite->getChildrenCount(); child++)
		{
			Sprite* spriteChild = (Sprite*)sprite->getChildren().at(child);
			Sprite* clone = DuplicateSprite((Sprite*)spriteChild);
			clone->boundingBox() = spriteChild->boundingBox();
			clone->setContentSize(spriteChild->getContentSize());
			clone->setPosition(spriteChild->getPosition());
			clone->setAnchorPoint(spriteChild->getAnchorPoint());
			pRet->addChild(clone, spriteChild->getZOrder(), spriteChild->getTag());
		}
	}
	return pRet;
}

Animate* ResourceManager::GetAnimate(std::string &dir, std::string &format, int count)
{
	int n = format.length();

	char *c = new char[n + 1];
	strcpy(c, format.c_str());

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(dir);
	auto spritecache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> animFrames;
	char str[100];
	for (int i = 1; i <= count; i++)
	{
		sprintf(str, c, i);
		animFrames.pushBack(spritecache->getSpriteFrameByName(str));
	}
	auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	return Animate::create(animation);
}
