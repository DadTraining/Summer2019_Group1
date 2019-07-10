#include "ResourceManager.h"

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

ResourceManager::ResourceManager()
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
