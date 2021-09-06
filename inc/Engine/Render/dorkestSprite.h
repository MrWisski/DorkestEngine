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
#include <Util/Math/Vector2.h>
#include <SFML/Graphics.hpp>

class dorkestSprite {
public:

	dorkestSprite() {}

	dorkestSprite(std::string Name,std::string spriteSheet) : m_name(Name), m_sourcesheet(spriteSheet) {
		
	
	}

	dorkestSprite(dorkestSprite& other) {
		m_name = other.m_name;
		m_sourcesheet = other.m_sourcesheet;
		m_spriteCoord = other.m_spriteCoord;
		m_spriteSize = other.m_spriteSize;
		m_color = other.m_color;
	}

	void setSpriteSize(Vector2d s) { m_spriteSize = s; }
	Vector2d getSpriteSize() { return m_spriteSize; }

	void setSpriteCoord(Vector2d coord) { m_spriteCoord = coord; }
	Vector2i getSpriteCoord() { return m_spriteCoord; }

	void setScaleFactor(float scale) { m_scaleFactor = scale; }
	float getScaleFactor() { return m_scaleFactor; }

	void setSpriteTint(Colorf tint) { m_color = tint; }
	Colorf getSpriteTint() { return m_color; }

	void setAltGroup(std::string groupName) { m_altGroup = groupName; }
	std::string getAltGroup() { return m_altGroup; }

	void set3x3GroupName(std::string groupName) { m_3x3GroupName = groupName; }
	std::string get3x3GroupName() { return m_3x3GroupName; }

	void setIs3x3(bool newval) { m_is3x3 = newval; }
	bool getIs3x3() { return m_is3x3; }

	bool hasSprite() { return (m_sourcesheet != ""); }
	std::string getSpriteSheet() { return m_sourcesheet; }

	std::string toStr() {
		std::string ret = "SPRITE CONTENTS : \n";
		ret += "Name : " + m_name + "\n";
		ret += "Scale Factor: " + std::to_string(m_scaleFactor) + "\n";
		
		if ((this->m_sourcesheet == "")) {
			ret += "Sprite Sheet null : Yes\n";

		}
		else {
			ret += "Sprite Sheet null : No\n";

		}

		ret += "Sprite Coord : " + m_spriteCoord.toStr() + "\n";
		ret += "Sprite Size : " + m_spriteSize.toStr() + "\n";
		
		return ret;

	}

	sf::Sprite* getSprite();

	~dorkestSprite() {
		if (m_spriteData != nullptr) {
			delete m_spriteData;
		}
	}
private:
	std::string m_name = "";
	float m_scaleFactor = 1.0f;
	std::string m_sourcesheet = "";
	
	sf::Sprite* m_spriteData = nullptr;

	Vector2i m_spriteCoord = {0, 0 };
	Vector2i m_spriteSize = { 0, 0 };
	Colorf m_color = Colorf(1,1,1,1);

	std::string m_altGroup = "";

	std::string m_3x3GroupName = "";
	bool m_is3x3 = false;
};