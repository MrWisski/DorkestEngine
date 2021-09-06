#include <Engine/Render/dorkestSprite.h>
#include <Engine/Render/dorkestSpriteManager.h>

sf::Sprite* dorkestSprite::getSprite() {
	if (m_spriteData == nullptr) {
		m_spriteData = new sf::Sprite();
		m_spriteData->setTexture(*(dorkestSpriteMan::getInstance()->getSheet(this->m_sourcesheet)->getSpriteSheet()));
		m_spriteData->setTextureRect(sf::IntRect(sf::Vector2i(m_spriteCoord.x, m_spriteCoord.y), sf::Vector2i(m_spriteSize.x, m_spriteSize.y)));
		
		if (m_spriteData == nullptr) {
			error("Failed to create sprite.");
		}
			return m_spriteData;
	}
	else {
		return m_spriteData;
	}

}
