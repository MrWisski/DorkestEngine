#include <Engine/Render/dorkestSpriteSheet.h>
#include <Engine/Render/dorkestSpriteManager.h>

bool dorkestSpriteSheet::registerSprite(std::string name) {
	dorkestSpriteMan* dsm = dorkestSpriteMan::getInstance();
	std::string source = m_data[SOURCENAME];
	int r = m_data[SPRITEDATA][name][SPRITETINTR];
	int g = m_data[SPRITEDATA][name][SPRITETINTG];
	int b = m_data[SPRITEDATA][name][SPRITETINTB];
	int a = m_data[SPRITEDATA][name][SPRITETINTA];
	Colorf tint = Colorf(r, g, b, a);
	float scale = m_data[SPRITEDATA][name][SPRITESCALE];
	Vector2d size = Vector2d(m_data[SPRITESIZEW], m_data[SPRITESIZEH]);
	Vector2d loc = Vector2d(m_data[SPRITEDATA][name][SPRITELOCX], m_data[SPRITEDATA][name][SPRITELOCY]);
	std::string groupName = m_data[SPRITEDATA][name][SPRITEALTGROUP];
	if (dsm->makeNewDorkestSprite(source, name, tint, scale, size, loc, groupName)) {
		//force the sprite to load its sf::Sprite
		dsm->getDorkestSprite(name)->getSprite();
		return true;
	} 
	return  false;
}
