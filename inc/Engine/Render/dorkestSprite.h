/*
	Name : Dorkest Sprite
	Created : 5/26/2021
	Author : MrWisski
	License : (WISSKI-1)

	Copyright 2012 - 2021 Alex Wisnieski AKA MrWisski

	Permission is hereby granted, free of charge, to any person (hereafter "You") obtaining a copy or portion of this software, its resulting
	compilation products, or associated documentation files (hereafter "Software"), to deal in the Software with the following
	restrictions, using the following terminology :

	TERMINOLOGY :
		1) COMMERCIAL : Any use of the Software in which You, or others recieve compensation for any reason, of any kind.

	RESTRICTIONS :
		1) You MAY use, copy, publish, distribute, and/or merge, this Software, with any project or library or software,
		   that is not COMMERCIAL in nature or intent.

		2) You MAY NOT use, copy, publish, distribute, redistribute, sublicense, merge and/or sell copies of any software including partially,
		   containing whole, or any portion of the Software, nor permit persons to whom the Software is furnished to do so for COMMERCIAL
		   uses.

		3) Derivations of the Software are permitted, provided the above copyright notice and this permission notice are NOT
		   modified, with the exception of character encoding changes that do not substantially change the intent of the notice or the
		   above copyright notice, or the readibility or useability of this permission notice or the above copyright notice, except for
		   COMMERCIAL uses.

		4) You MAY NOT imply endorsement of any product that includes or derives from the Software, by any any person or group of people
		   involved in the creation of the Software without direct written permission from said persons or group of people.

		5) You MAY NOT use the legal names, handles, aliases, or nicknames of any person involved in the creation of the Software,
		   to promote any product derived from, or using the Software in any way, without written permission from said persons.

	The above copyright notice and this permission notice shall be included in all copies or portions of the Software.

	Any of the above restrictions which are unlawful or not permitted, whole or in part, in the jurisdiction You live in will not
	affect the rest of the portion of the restriction it exists in, the other restrictions listed in this permission notice.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

	CERTAIN JURISDICTIONS DO NOT ALLOW LIMITATIONS ON IMPLIED WARRANTIES OR THE EXCLUSION OR LIMITATION OF CERTAIN DAMAGES.
	IF YOU RESIDE IN SUCH A JURISDICTION, SOME OR ALL OF THE ABOVE DISCLAIMERS, EXCLUSIONS, OR LIMITATIONS MAY NOT APPLY TO YOU,
	AND YOU MAY HAVE ADDITIONAL RIGHTS. THE LIMITATIONS OR EXCLUSIONS OF WARRANTIES, REMEDIES, OR LIABILITY CONTAINED IN THESE TERMS
	APPLY TO YOU TO THE FULLEST EXTENT SUCH LIMITATIONS OR EXCLUSIONS ARE PERMITTED UNDER THE LAWS OF THE JURISDICTION IN WHICH
	YOU ARE LOCATED.
*/
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