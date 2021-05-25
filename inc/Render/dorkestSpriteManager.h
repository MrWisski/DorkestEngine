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

