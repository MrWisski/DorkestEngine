/*
	Name : Dorkest Sprite Manager
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
#include <map>
#include <string>
#include <Util/Log.h>

#include "dorkestSprite.h"


class dorkestSpriteMan {
public:
	static dorkestSpriteMan* getInstance() {
		if (dorkestSpriteMan::instance == 0) {
			dorkestSpriteMan::instance = new dorkestSpriteMan();
		}

		return dorkestSpriteMan::instance;
	}

	bool addNewSpriteSheet(std::string name, std::string path) {
		/*	it = mymap.find('b');
			if (it != mymap.end())
				mymap.erase (it);*/

		auto itr = m_spriteSheets.find(name);
		if (itr != m_spriteSheets.end()) {
			error("Cannot add a spritesheet with the name of " + name + " -- Spritesheet with that name already exists!");
			return false;
		}
		
		olc::Sprite* newSheet;
		newSheet = new olc::Sprite(path);

		if (newSheet == nullptr || newSheet == 0) {
			error("Could not load sprite from file!");
			return false;
		}

		m_spriteSheets.emplace(name, newSheet);

		//Confirm its actually in the map!
		itr = m_spriteSheets.find(name);
		if (itr != m_spriteSheets.end()) {
			debug("Spritesheet added successfully.");
			return true;
		} else {
			error("Failed to add spritesheet with name " + name + " to the spritesheets map!");
			delete(newSheet);
			return false;
		}
	}

	bool makeDecal(std::string spriteSourceName) {
		auto itr = m_spriteSheets.find(spriteSourceName);
		if (itr == m_spriteSheets.end()) {
			error("Cannot add a decalsheet with the name of " + spriteSourceName + " -- Spritesheet with that name does not exist! Add it first and try again!");
			return false;
		}
		std::string name = itr->first;
		if (itr->second == nullptr || itr->second == 0) {
			error("CRITICAL ERROR - iterator returned pointer of 0/null");
			return false;
		}
		olc::Decal* newSheet = new olc::Decal(itr->second);

		if (newSheet != nullptr && newSheet != 0) {
			m_decalSheets.emplace(name, newSheet);
		} else {
			error("Failed to create new decal from spritesheet " + name + "!");
			delete newSheet;
			return false;
		}

		
		//confirm it's actually in the map!
		auto itrD = m_decalSheets.find(name);

		if (itrD != m_decalSheets.end()) {
			debug("New Decalsheet created successfully.");
			return true;
		}
		else {
			error("Failed to add spritesheet with name " + name + " to the spritesheets map!");
			delete(newSheet);
			return false;
		}		
	}

	bool makeDorkestSprite_Decal(std::string dorkestSpriteName, olc::Pixel tint, std::string decalSheetName, float scale, olc::vi2d size, olc::vi2d spriteSrcCoord) {
		auto itr = m_dSprites.find(dorkestSpriteName);
		if (itr != m_dSprites.end()) {
			error("Cannot add a decalsheet with the name of " + dorkestSpriteName + " -- dorkestSprite with that name already exists!");
			return false;
		}


		auto itrdecal = m_decalSheets.find(decalSheetName);
		if (itrdecal == m_decalSheets.end()) {
			error("Cannot create the dSprite " + dorkestSpriteName + " -- cannot find decal sheet " + decalSheetName + "! Create it first with dorkestSpriteMan::getInstance()->makeDecal(" + decalSheetName + ")!");
			return false;
		}

		dorkestSprite* newSprite = new dorkestSprite(dorkestSpriteName,itrdecal->second);
		newSprite->setSpriteCoord(spriteSrcCoord);
		newSprite->setScaleFactor(scale);
		newSprite->setSpriteTint(tint);
		newSprite->setSpriteSize(size);

		m_dSprites.emplace(dorkestSpriteName, newSprite);
		
		//confirm it's actually in the map!
		auto itrD = m_dSprites.find(dorkestSpriteName);

		if (itrD != m_dSprites.end()) {
			debug("Created dorkestSprite " + dorkestSpriteName+ ".");
			return true;
		}
		else {
			error("Failed to add dorkestsprite with name " + dorkestSpriteName + " to the sprite map!");
			delete newSprite;
			return false;
		}
	}

	dorkestSprite* getDorkestSprite(std::string name) {
		auto itr = m_dSprites.find(name);
		if (itr == m_dSprites.end()) {
			error("Cannot find a dorkestSprite with the name of " + name + "!");
			return nullptr;
		}

		return itr->second;
	}

private:
	static dorkestSpriteMan* instance;
	dorkestSpriteMan() {}
	~dorkestSpriteMan() {
		for (auto p : m_dSprites) {
			delete(p.second);
		}
		m_dSprites.clear();

		for (auto p : m_spriteSheets) {
			delete(p.second);
		}
		m_spriteSheets.clear();

		for (auto p : m_decalSheets) {
			delete(p.second);
		}
		m_decalSheets.clear();

		dorkestSpriteMan::instance = 0;

	}

	std::map<std::string, dorkestSprite*> m_dSprites;
	std::map<std::string, olc::Sprite*> m_spriteSheets;
	std::map<std::string, olc::Decal*> m_decalSheets;

	


};

