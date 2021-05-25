#pragma once

#include <olcPixelGameEngine.h>
#include <instPGE.h>




class dorkestSprite {
public:

	dorkestSprite() {}

	dorkestSprite(std::string Name, olc::Sprite* spriteSheet) : m_name(Name), m_sourcesheet(spriteSheet) {}
	dorkestSprite(std::string Name, olc::Decal* decalSheet) {
		m_name = Name;
		m_decalsheet = decalSheet;
	}

	dorkestSprite(dorkestSprite& other) {
		m_name = other.m_name;
		m_sourcesheet = other.m_sourcesheet;
		m_decalsheet = other.m_decalsheet;
		m_spriteCoord = other.m_spriteCoord;
		m_spriteSize = other.m_spriteSize;
		m_color = other.m_color;
	}

	void setSpriteSize(olc::vi2d s) { m_spriteSize = s; }
	olc::vi2d getSpriteSize() { return m_spriteSize; }

	void setSpriteCoord(olc::vi2d coord) { m_spriteCoord = coord; }
	olc::vi2d getSpriteCoord() { return m_spriteCoord; }

	void setScaleFactor(float scale) { m_scaleFactor = scale; }
	float getScaleFactor() { return m_scaleFactor; }

	void setSpriteTint(olc::Pixel tint) { m_color = tint; }
	olc::Pixel getSpriteTint() { return m_color; }

	bool hasDecal() { return (m_decalsheet != nullptr); }
	olc::Decal* getDecalSheet() { return m_decalsheet; }

	bool hasSprite() { return (m_sourcesheet != nullptr); }
	olc::Sprite* getSpriteSheet() { return m_sourcesheet; }

	std::string toStr() {
		std::string ret = "SPRITE CONTENTS : \n";
		ret += "Name : " + m_name + "\n";
		ret += "Scale Factor: " + std::to_string(m_scaleFactor) + "\n";
		
		if ((this->m_decalsheet == nullptr)) {
			ret += "Decal Sheet null : Yes\n";

		}
		else {
			ret += "Decal Sheet null : No\n";

		}
		
		if ((this->m_sourcesheet == nullptr)) {
			ret += "Sprite Sheet null : Yes\n";

		}
		else {
			ret += "Sprite Sheet null : No\n";

		}

		ret += "Sprite Coord : " + m_spriteCoord.str() + "\n";
		ret += "Sprite Size : " + m_spriteSize.str() + "\n";
		
		return ret;

	}

private:
	std::string m_name = "";
	float m_scaleFactor = 1.0f;
	olc::Sprite* m_sourcesheet = nullptr;
	olc::Decal* m_decalsheet = nullptr;

	olc::vi2d m_spriteCoord = {0, 0 };
	olc::vi2d m_spriteSize = { 0, 0 };
	olc::Pixel m_color = olc::WHITE;

};