#pragma once

#include <Util/JSON.h>
#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <Engine/Render/dorkestSprite.h>

#include <fstream>
#include <filesystem>

using namespace nlohmann;

#define SHEETSAVEPATH "sheetSavePath"
#define SOURCENAME "sourceName"
#define SOURCEPATH "sourcePath"
#define SOURCEWIDTH "sourceWidth"
#define SOURCEHEIGHT "sourceHeight"
#define SOURCECELLPADDING "sourceCellPadding"
#define SPRITESIZEW "spriteSizeW"
#define SPRITESIZEH "spriteSizeH"
#define SPRITECOUNT "spriteCount"


#define SPRITEDATA "spriteData"

#define SPRITENAME "spriteName"
#define SPRITELOCX "spriteCellX"
#define SPRITELOCY "spriteCellY"
#define SPRITETINTR "spriteTintR"
#define SPRITETINTG "spriteTintG"
#define SPRITETINTB "spriteTintB"
#define SPRITETINTA "spriteTintA"
#define SPRITESCALE "spriteScale"
#define SPRITEALTGROUP "spriteAltGroup"
#define IS3X3 "is3x3"
#define GROUPNAME3X3 "GroupName3x3"


class dorkestSpriteSheet {
public:

	json getData() { return m_data; }

	void print() {
		debugss << "Name : " << m_data[SOURCENAME] << std::endl;
		debugss << "Cell Padding : " << m_data[SOURCECELLPADDING] << std::endl;
		debugss << "Source Image Height : " << m_data[SOURCEHEIGHT] << std::endl;
		debugss << "Source Image Width : " << m_data[SOURCEWIDTH] << std::endl;
		debugss << "Sheet Save Path : " << m_data[SHEETSAVEPATH] << std::endl;
		debugss << "Sprite Size H : " << m_data[SPRITESIZEH] << std::endl;
		debugss << "Sprite Size W : " << m_data[SPRITESIZEW] << std::endl;
		debugss << "Sprite Count : " << m_data[SPRITECOUNT] << std::endl;
		
	}

	void printSprite(std::string name) {
		debug("");


	}

	dorkestSpriteSheet(std::string name) {
		m_data[SOURCENAME] = name;
		m_data[SOURCECELLPADDING] = 0;
		m_data[SOURCEHEIGHT] = 0;
		m_data[SOURCEWIDTH] = 0;
		m_data[SHEETSAVEPATH] = "";
		m_data[SPRITESIZEH] = 0;
		m_data[SPRITESIZEW] = 0;
		m_data[SPRITECOUNT] = 0;
	}

	//load the sheet from a json data file.
	dorkestSpriteSheet(std::string path, std::string filename) {
		debug("Loading sheet from " + path + filename);
		std::ifstream i(path + filename);
		i >> m_data;
		i.close();

		debug("Got data : ");
		this->print();
		m_sourcesheet = new sf::Texture();

		bool ret = this->m_sourcesheet->loadFromFile(m_data[SOURCEPATH]);

		if (!ret) {
			error("FAILED TO LOAD SHEET!");
			return;

		}
		m_sourcesheet->setSmooth(false);
		m_sourcesheet->setRepeated(false);
		m_sourcesheet->setSrgb(true);

	}

	void registerSprites() {
		int count = 0;
		if (m_data.count(SPRITECOUNT) != 0) {
			if (!m_data.is_null() && !m_data[SPRITECOUNT].is_null())
				count = m_data[SPRITECOUNT];
			else
				count = 0;
		}
		else
			count = 0;

		this->m_spriteMap.clear();

		if (count > 0) {
			debug("Count is greater than zero -> " + std::to_string(count));
			size_t actual = m_data[SPRITEDATA].size();
			if (count == actual) {

			}
			else {
				error("Spritesheet sprite count is not equal to actual # of sprites.");
			}

			json o = m_data[SPRITEDATA];
			for (json::iterator it = o.begin(); it != o.end(); ++it) {

				std::string msg("Registering sprite " + std::string(it.key()));
				debug(msg);
				this->registerSprite(std::string(it.key()));
				int sx = m_data[SPRITEDATA][std::string(it.key())][SPRITELOCX];
				int sy = m_data[SPRITEDATA][std::string(it.key())][SPRITELOCY];
				debug("Inserting : " + Vector2i(sx, sy).toStr() + " at " + it.key());

				this->m_spriteMap.insert(std::pair(Vector2i(sx, sy), std::string(it.key())));
			}

		}
		else {
			debug("Sprite sheet json is empty.");
		}
	}


	std::string getName() { return m_data[SOURCENAME]; }
	bool save() {
		std::string path = m_data[SHEETSAVEPATH];
		std::ofstream o(path);
		o << std::setw(4) << m_data << std::endl;
		o.close();
		return true;
	}

	~dorkestSpriteSheet() {
		debug("");
		debug("Saving on deconstructor");
		this->save();
	}

	bool setSavePath(std::string path) {
		debug("");
		m_data[SHEETSAVEPATH] = path;
		return true;
	}

	bool setSourceName(std::string name) {
		debug("");
		m_data[SOURCENAME] = name;
		return true;
	}

	bool setSourceImage(std::string path) {
		debug("");
		m_data["sourcePath"] = path;
		if (this->m_sourcesheet == nullptr) {
			this->m_sourcesheet = new sf::Texture();
		}

		if (!this->m_sourcesheet->loadFromFile(path)) {
			error("FAILED TO LOAD SHEET!");
			return false;

		}
		m_data[SOURCEWIDTH] = this->m_sourcesheet->getSize().x;
		m_data[SOURCEHEIGHT] = this->m_sourcesheet->getSize().y;
		return true;


	}

	bool setSourceCellPadding(int cellPadding) {
		m_data[SOURCECELLPADDING] = cellPadding;
		return true;
	}

	bool setSourceSpriteSize(int w, int h) {
		m_data[SPRITESIZEH] = h;
		m_data[SPRITESIZEW] = w;
		return true;
	}

	bool setSpriteCount(int spriteCount) {
		m_data[SPRITECOUNT] = spriteCount;
		return true;
	}

	bool defineSprite(std::string name, dorkestSprite* spriteDef) {
		if (m_data[SPRITEDATA].contains(name)) {
			error("Cannot define sprite named " + name + " - sprite already exists in the sheet!");
			return false;
		}
		//m_data[SPRITEDATA][name] = name;
		m_data[SPRITEDATA][name][SPRITELOCX] = spriteDef->getSpriteCoord().x;
		m_data[SPRITEDATA][name][SPRITELOCY] = spriteDef->getSpriteCoord().y;
		m_data[SPRITEDATA][name][SPRITETINTR] = spriteDef->getSpriteTint().r;
		m_data[SPRITEDATA][name][SPRITETINTG] = spriteDef->getSpriteTint().g;
		m_data[SPRITEDATA][name][SPRITETINTB] = spriteDef->getSpriteTint().b;
		m_data[SPRITEDATA][name][SPRITETINTA] = spriteDef->getSpriteTint().a;
		m_data[SPRITEDATA][name][SPRITESCALE] = spriteDef->getScaleFactor();
		m_data[SPRITEDATA][name][SPRITEALTGROUP] = spriteDef->getAltGroup();
		m_data[SPRITEDATA][name][IS3X3] = spriteDef->getIs3x3();
		m_data[SPRITEDATA][name][GROUPNAME3X3] = spriteDef->get3x3GroupName();

		debug("Setting Sprite at " + Vector2i(spriteDef->getSpriteCoord().x, spriteDef->getSpriteCoord().y).toStr() + " = " + name);

		int c = m_data[SPRITECOUNT];
		m_data[SPRITECOUNT] = c + 1;
		m_spriteMap.insert_or_assign(Vector2i(spriteDef->getSpriteCoord().x, spriteDef->getSpriteCoord().y),name);
		debug("Defined sprite named " + name);
		return true;
	}

	bool registerSprite(std::string name);

	sf::Texture* getSpriteSheet() { return m_sourcesheet; }

	std::string getSpriteAt(int cellx, int celly) {
		//Translate the cell pos to actual spritesheet coordinates
		int posx = (cellx * (m_data[SPRITESIZEW])) + ((m_data[SOURCECELLPADDING]) * cellx);
		int posy = (celly * (m_data[SPRITESIZEH])) + ((m_data[SOURCECELLPADDING]) * celly);

		int c = this->m_spriteMap.count(Vector2i(posx, posy));
		debug("Count of " + Vector2i(posx, posy).toStr() +" is " + std::to_string(c));
		dumpSpritemap();

		//Get a count of the sprites (should be > 0)
		if (m_spriteMap.count(Vector2i(posx, posy))) {
			auto itr = m_spriteMap.find(Vector2i(posx, posy));
			return itr->second;

		}
		else {
			debug("No sprite at " + std::to_string(cellx) + "," + std::to_string(celly));
			return "";
		}
	}

	void dumpSpritemap() {
		for (auto p : m_spriteMap) {
			debug("First : " + Vector2i(p.first.x,p.first.y).toStr() + "  Second : " + p.second);
		}
	}

private:

	sf::Texture* m_sourcesheet;
	json m_data;
	std::map<Vector2i, std::string> m_spriteMap;
};