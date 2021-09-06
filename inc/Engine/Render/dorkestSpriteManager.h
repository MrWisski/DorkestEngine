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
#include "dorkestSpriteSheet.h"
#include <Util/Math/Vector2.h>
#include <Util/Color.h>


class dorkestSpriteMan {
public:
	static dorkestSpriteMan* getInstance() {
		if (dorkestSpriteMan::instance == 0) {
			dorkestSpriteMan::instance = new dorkestSpriteMan();
		}

		return dorkestSpriteMan::instance;
	}


	/// <summary>
	/// Loads a spritesheet from disk in JSON format.
	/// </summary>
	/// <param name="path">Path to the file</param>
	/// <param name="filename">Name of file to load.</param>
	/// <returns>The name of the sheet to use for access.</returns>
	std::string loadSpriteSheet(std::string path, std::string filename) {
		debug("Loading SS from .json");
		dorkestSpriteSheet* newsheet = new dorkestSpriteSheet(path + "/", filename);
		auto itr = m_spriteSheets.find(newsheet->getName());
		if (itr != m_spriteSheets.end()) {
			error("Sprite sheet named " + newsheet->getName() + " already exists. If you wish to replace, use replaceSpriteSheet()");
			return "";
		}
		debug("Adding sprite sheet " + newsheet->getName() + " to SS index.");
		this->m_spriteSheets.emplace(newsheet->getName(), newsheet);
		newsheet->registerSprites();
		return newsheet->getName();
	}

	/// <summary>
	/// Loads a spritesheet from disk in JSON format, replacing any existing copy in the index.
	/// </summary>
	/// <param name="path">Path to the file</param>
	/// <param name="filename">Name of file to load.</param>
	/// <returns>The name of the sheet to use for access.</returns>
	std::string replaceSpriteSheet(std::string path, std::string filename) {
		debug("Loading SS from .json");
		dorkestSpriteSheet* newsheet = new dorkestSpriteSheet(path + "\\", filename);
		if (newsheet == nullptr || newsheet->getName() == "") {
			error("Invalid sheet - new returned null, or name is blank.");
			return "";
		}
		auto itr = m_spriteSheets.find(newsheet->getName());
		bool replace = false;
		if (itr != m_spriteSheets.end()) {
			debug("Replacing existing copy.");
			replace = true;
		}

		if (replace) {
			m_spriteSheets.insert_or_assign(newsheet->getName(), newsheet);
		} else {
			this->m_spriteSheets.emplace(newsheet->getName(), newsheet);
		}
		
		newsheet->registerSprites();
		return newsheet->getName();
	}


	dorkestSpriteSheet* getSheet(std::string name) {
		auto itr = m_spriteSheets.find(name);
		if (itr == m_spriteSheets.end()) {
			error("Sprite sheet named " + name + " was not found.");
			return nullptr;
		}
		return itr->second;

	}

	bool addNewSpriteSheet(std::string name, std::string path, int spritex, int spritey, int padding) {
		/*	it = mymap.find('b');
			if (it != mymap.end())
				mymap.erase (it);*/
		debug("dsm->addnewSS");
		auto itr = m_spriteSheets.find(name);
		if (itr != m_spriteSheets.end()) {
			error("Cannot add a spritesheet with the name of " + name + " -- Spritesheet with that name already exists!");
			return false;
		}

		dorkestSpriteSheet* newSheet;
		newSheet = new dorkestSpriteSheet(name);
		debug("Source image path = " + path);
		newSheet->setSourceSpriteSize(spritex, spritey);
		newSheet->setSourceCellPadding(padding);
		newSheet->setSourceImage(path);
		
		

		if (newSheet == nullptr) {
			error("Could not load sprite from file!");
			return false;
		}

		m_spriteSheets.emplace(name, newSheet);

		//Confirm its actually in the map!
		itr = m_spriteSheets.find(name);
		if (itr != m_spriteSheets.end()) {
			debug("Spritesheet added successfully.");
			return true;
		}
		else {
			error("Failed to add spritesheet with name " + name + " to the spritesheets map!");
			delete(newSheet);
			return false;
		}
	}



	bool makeNewDorkestSprite(std::string SheetName, std::string dorkestSpriteName, Colorf tint, float scale, Vector2d size, Vector2d spriteSrcCoord, std::string groupName) {
		auto itr = m_dSprites.find(dorkestSpriteName);
		if (itr != m_dSprites.end()) {
			error("Cannot add a decalsheet with the name of " + dorkestSpriteName + " -- dorkestSprite with that name already exists!");
			return false;
		}


		auto itrdecal = m_spriteSheets.find(SheetName);
		if (itrdecal == m_spriteSheets.end()) {
			error("Cannot create the dSprite " + dorkestSpriteName + " -- cannot find sprite sheet " + SheetName + "!");
			return false;
		}

		dorkestSprite* newSprite = new dorkestSprite(dorkestSpriteName, itrdecal->second->getName());
		newSprite->setSpriteCoord(spriteSrcCoord);
		newSprite->setScaleFactor(scale);
		newSprite->setSpriteTint(tint);
		newSprite->setSpriteSize(size);
		newSprite->setAltGroup(groupName);
	
		m_dSprites.emplace(dorkestSpriteName, newSprite);

		//confirm it's actually in the map!
		auto itrD = m_dSprites.find(dorkestSpriteName);

		if (itrD != m_dSprites.end()) {
			debug("Created dorkestSprite " + dorkestSpriteName + ".");
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

		dorkestSpriteMan::instance = 0;

	}

	std::map<std::string, dorkestSprite*> m_dSprites;
	std::map<std::string, dorkestSpriteSheet*> m_spriteSheets;




};

