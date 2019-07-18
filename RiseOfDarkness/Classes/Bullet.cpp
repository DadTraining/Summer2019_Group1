#include "Bullet.h"
#include "ResourceManager.h"

USING_NS_CC;

Bullet::Bullet(Layer * layer)
{
	this->Init();
	m_sprite->removeFromParent();
	layer->addChild(m_sprite, 1);
}

Bullet::~Bullet()
{
}

Sprite * Bullet::GetSprite()
{
	return this->m_sprite;;
}

Sprite * Bullet::CloneSprite(Sprite * sprite)
{
	Sprite *spriteCl = Sprite::createWithTexture(sprite->getTexture());
	return spriteCl;
}

void Bullet::Init()
{
	m_sprite = ResourceManager::GetInstance()->DuplicateSprite(ResourceManager::GetInstance()->GetSpriteById(25));
	m_sprite->setScale(0.5f);
	//m_sprite->removeFromParent();
	m_sprite->setVisible(false);
}

void Bullet::Update(float deltaTime, Vec2 posMt)
{
	float dis = sqrt(pow((m_sprite->getPosition().x - posMt.x), 2)
		+ pow((m_sprite->getPosition().y - posMt.y), 2));
	auto visibleSize = Director::getInstance()->getVisibleSize();
	if (m_sprite->isVisible())
	{
		m_sprite->setPositionY(m_sprite->getPositionY() + 5);
	}
	if (dis > 100)
	{
		this->m_sprite->setVisible(false);
	}
}
