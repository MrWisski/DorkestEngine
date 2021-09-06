#define OLC_PGEX_SOUND
#define OLC_PGEX_AUDIOCONVERT

#include <sheetEditor.h>

#include <SFML/Window.hpp>

#include <Engine/Render/dorkestSpriteManager.h>
#include "Util/Math/Vector4.h"
#include "ECS/components.h"
#include "Util/dorkestProfiler.h"
#include <Engine/Render/dorkestRenderer.h>
#include "ECS/dorkestScene.h"

#include "Util/Math/Vector2.h"

#include "Util/Log.h"
#include <Util/Math/Geometry/FibSphere.h>
#include <Util/Color.h>

#include <imgui.h>
#include <imgui-SFML.h>

bool sheetEditor::pressAnyKey() {

	return false;
}
bool sheetEditor::initSheetEditor() {

	if (!initWindow()) { return false; }

	dsm = dorkestSpriteMan::getInstance();
	this->m_camera = new dorkestCamera(1024, 512, 0, 0);
	this->m_renderer = new dorkestRenderer(m_window, m_camera);
	pos.x = 0;
	pos.y = 0;
	m_camera->setTopDownView();
	m_camera->setMapSize(2048 * 2, 2048 * 2);
	m_camera->setMapCorner(0, 0);

	m_camera->setCenter(512, 256);
	m_camera->render(m_window);
	this->m_useFont = new sf::Font();
	if (!this->m_useFont->loadFromFile("./res/Fonts/Kenney Future.ttf")) {
		error("Error loading font from file!");
		return false;
	}

	//debug("Loaded Sprite Sheet : " + dsm->loadSpriteSheet("./res", "roguelike.json"));
	this->m_camera->setScreenSize(1024-300, 512);

	ImGui::SFML::Init(*m_window);

	this->toolwin = new DebugStuff();
	this->spriteWin = new spriteInfo();
	this->sheetDataWin = new sheetData();
	this->resizeCursor();
	this->lsX = this->sheetDataWin->sX;
	this->lsY = this->sheetDataWin->sY;

	return true;

}

void sheetEditor::resizeCursor() {
/*
	if (this->activeSS == nullptr) { debug("activess is null."); return; }

	

	int x = activeSS->getData()[SPRITESIZEW];
	int y = activeSS->getData()[SPRITESIZEH];
	sf::RectangleShape box;
		box.setPosition(sf::Vector2f(0, 0));
		box.setSize(sf::Vector2f((float)x, (float)y));
		box.setFillColor(sf::Color(0, 0, 0, 0));
		box.setOutlineColor(sf::Color::White);
		box.setOutlineThickness(3);
	debug("x,y = " + Vector2i(x, y).toStr());
	if (this->m_cursorT == nullptr) {
		debug("Setting up cursor texture.");
		m_cursorT = new sf::RenderTexture();

		m_cursorT->create(x, y);
		m_cursorT->clear();
		
		m_cursorT->draw(box);
		m_cursorT->display();

	}

	if (this->m_cursor == nullptr) {
		m_cursor = new sf::Sprite(m_cursorT->getTexture());
	}
	

	*/
}

bool sheetEditor::initWindow() {
	if (dsm != nullptr) {
		error("Call initWindow() before initsheetEditor()!");
		return false;
	}
	m_window = new sf::RenderWindow(sf::VideoMode(1024, 512), "Dorkest sheetEditor", sf::Style::Default);
	if (m_window == nullptr) {
		error("Failed to create window!");
		return false;
	}
	m_window->setVisible(true);

	debug("Setting up window input manager.");
	this->im = inputMan::getInstance();
	im->setWindow(m_window);
	im->getBus()->subscribe(this, &sheetEditor::handleWinEvent);
	im->getBus()->subscribe(this, &sheetEditor::handleKeyEvent);
	im->getBus()->subscribe(this, &sheetEditor::handleMouseEvent);

	return true;

}

void sheetEditor::handleKeyEvent(keyEvent* e) {
	if (ImGui::GetIO().WantCaptureKeyboard) {
		return;
	}

	if (!e->action) return;
	debug("got keypress.");
	switch (e->key) {
	case sf::Keyboard::W:
		cursorMoved();
		celly--;
		if (celly < 0) celly = 0;
		updateSpriteWin();
		break;
	case sf::Keyboard::A:
		cursorMoved();
		cellx--;
		if (cellx < 0) cellx = 0;
		updateSpriteWin();
		break;
	case sf::Keyboard::S:
		cursorMoved();
		celly++;

		updateSpriteWin();
		//if (pos.y >= m_camera->getMapSize().y - activeSS->getData()[SPRITESIZEH]) pos.y = m_camera->getMapSize().y - activeSS->getData()[SPRITESIZEH];
		break;
	case sf::Keyboard::D:
		cursorMoved();
		cellx++;
		updateSpriteWin();
		break;
	case sf::Keyboard::Escape:
		break;
	}
}
void sheetEditor::handleMouseEvent(mouseEvent* e) {
	if (ImGui::GetIO().WantCaptureMouse) {
		return;
	}

	switch (e->eType) {
	case mouseEventType::WHEEL:
		debug("Recieved wheel event.");
		if (e->delta > 0) m_camera->zoomIn();
		if (e->delta < 0) m_camera->zoomOut();
		break;

	}


}

void sheetEditor::handleWinEvent(windowEvent* e) {
	sf::FloatRect visibleArea;

	switch (e->eType) {
	case winEvent::CLOSING:
		debug("Recieved close event.");
		m_window->close();
		return;
		break;
	case winEvent::RESIZE:
		debug("Recieved resize event.");
		visibleArea = sf::FloatRect(0.f, 0.f, e->newSize.x, e->newSize.y);
		this->m_camera->setScreenSize(e->newSize.x, e->newSize.y);
		break;
	case winEvent::LOSTFOCUS:
		debug("Recieved lost focus event.");
		break;
	case winEvent::GAINEDFOCUS:
		debug("Recieved gained focus event.");
		break;

	}

}
bool sheetEditor::initSound() {
	return true;
}

bool sheetEditor::update(float fElapsedTime) {
	im->update();
	if (!m_window->isOpen()) return false;
	m_camera->update(pos, fElapsedTime);
	m_camera->render(m_window);

	ImGui::SFML::Update(*m_window, m_dt.restart());


	if (activeSS != nullptr && sheetDataWin->valchanged) {
		debug("Value changed -> " + std::to_string(this->sheetDataWin->sX) + ", " + std::to_string(this->sheetDataWin->sY));
		activeSS->setSourceSpriteSize(this->sheetDataWin->sX, this->sheetDataWin->sY);
		activeSS->setSourceCellPadding(this->sheetDataWin->padding);
		debug("Changed padding to " + std::to_string(this->sheetDataWin->padding));
		resizeCursor();
		sheetDataWin->valchanged = false;
	}

	int toolbarSpace = 20;

	m_renderer->clearWindow();

	if (activeSS != nullptr) {
		
		sf::Texture* dec = activeSS->getSpriteSheet();
		if (dec != nullptr) {
			sf::Sprite sheetSprite = sf::Sprite(*dec, sf::IntRect(0, 0, dec->getSize().x, dec->getSize().y));
			sheetSprite.setPosition(0, toolbarSpace);
			m_window->draw(sheetSprite);

			scale = m_camera->getZoom();

			int x = activeSS->getData()[SPRITESIZEW];
			int y = activeSS->getData()[SPRITESIZEH];
			sf::RectangleShape box;
			int posx = (cellx * (activeSS->getData()[SPRITESIZEW] * scale)) + ((activeSS->getData()[SOURCECELLPADDING] * scale) * cellx);
			int posy = (celly * (activeSS->getData()[SPRITESIZEH] * scale)) + toolbarSpace + ((activeSS->getData()[SOURCECELLPADDING] * scale) * celly);

			box.setPosition(sf::Vector2f(posx, posy));
			box.setSize(sf::Vector2f((float)x, (float)y));
			box.setFillColor(sf::Color(0, 0, 0, 0));
			int valr = (rand() % 255);
			int valg = (rand() % 255);
			int valb = (rand() % 255);

			box.setOutlineColor(sf::Color(valr,valg,valb));
			box.setOutlineThickness(3);

			
			//m_cursor->setScale(sf::Vector2f(scale, scale));
			//debug("Drawing cursor at " + Vector2i(posx, posy).toStr());
			m_window->draw(box);

			//pge->DrawSprite({ 0,(float)toolbarSpace }, dec->sprite, scale);

			
			//m_renderer->drawSprite({ 0,(int)toolbarSpace }, dec, { scale,scale });

			
			//debug(std::to_string(posx) + ", " + std::to_string(posy));


		}
		else {
			//error("DECAL IS NULL!");
		}
	}

	//Create the black area that houses the sprite info window AND the zoomed in highlighted sprite.
	sf::RectangleShape bgR;
	bgR.setSize({ 300,(float)m_window->getSize().y });
	bgR.setPosition({ (float)m_window->getSize().x - 300, 0 });
	bgR.setFillColor(sf::Color::Black);
	bgR.setOutlineColor(sf::Color::Green);
	bgR.setOutlineThickness(2.0f);
	m_window->draw(bgR);

	//Draw the zoomed in sprite.
	if (activeSS != nullptr) {
	
		sf::Texture* dec = activeSS->getSpriteSheet();
		if (dec != nullptr) {
			int slx = (this->cellx * activeSS->getData()[SPRITESIZEW]) + (cellx * activeSS->getData()[SOURCECELLPADDING]);
			int sly = (celly * activeSS->getData()[SPRITESIZEH]) + (celly * activeSS->getData()[SOURCECELLPADDING]);

			m_zoomSprite = new sf::Sprite(*dec, sf::IntRect(slx, sly, activeSS->getData()[SPRITESIZEW], activeSS->getData()[SPRITESIZEH]));
			m_zoomSprite->setPosition((m_window->getSize().x - 300) + 150 - this->sheetDataWin->sX, 300 + this->sheetDataWin->sY);
			m_zoomSprite->setScale({ 6.0f,6.0f });
			//pge->DrawSprite({ 0,(float)toolbarSpace }, dec->sprite, scale);

			m_window->draw(*m_zoomSprite);
		}
	}

	loadSSfileDialog.Display();
	loadImagefileDialog.Display();
	saveSSfileDialog.Display();
	this->toolwin->render();
	Vector2i si = { (int)m_window->getSize().x,(int)m_window->getSize().y };
	this->spriteWin->render(si);
	spriteWin->show();
	sheetDataWin->render(si);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load New Image"))
			{

				loadImagefileDialog.SetTitle("Open sprite sheet base image");
				loadImagefileDialog.SetTypeFilters({ ".png",".jpg",".bmp" });
				loadImagefileDialog.Open();
				fbOpen = true;
				this->loadimage = true;

			}
			if (ImGui::MenuItem("Load Spritesheet"))
			{

				loadSSfileDialog.SetTitle("Load sprite sheet data file");
				loadSSfileDialog.SetTypeFilters({ ".json" });
				loadSSfileDialog.Open();
				fbOpen = true;
				this->loadSS = true;

			}
			if (ImGui::MenuItem("Save Spritesheet"))
			{

				saveSSfileDialog.SetTitle("Save Spritesheet");
				loadSSfileDialog.SetTypeFilters({ ".json" });
				saveSSfileDialog.Open();
				fbOpen = true;
				this->saveSS = true;

			}
			if (ImGui::MenuItem("Exit"))
			{
				ImGui::EndMenu();
				ImGui::EndMainMenuBar();
				return false;
			}


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Sheet")) {
			if (ImGui::MenuItem("Set Sprite Defaults")) {
				sheetDataWin->show();

			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();


		if (fbOpen && loadImagefileDialog.HasSelected() && this->loadimage) {

			debug(std::string(loadImagefileDialog.GetPwd().string()));
			debug("loadimage : " + loadImagefileDialog.GetSelected().filename().string());
			bool retval = this->newSpritesheet(loadImagefileDialog.GetPwd().string() + "\\", loadImagefileDialog.GetSelected().filename().string());
			fbOpen = false;
			loadimage = false;
			resizeCursor();

		}
		if (fbOpen && loadSSfileDialog.HasSelected() && this->loadSS) {

			debug(std::string(loadSSfileDialog.GetPwd().string()));
			debug("loadSS : " + loadSSfileDialog.GetSelected().filename().string());
			this->loadSpritesheet(loadSSfileDialog.GetPwd().string(), loadSSfileDialog.GetSelected().filename().string());
			fbOpen = false;
			loadSS = false;
			this->sheetDataWin->sX = activeSS->getData()[SPRITESIZEW];
			this->sheetDataWin->sY = activeSS->getData()[SPRITESIZEH];
			resizeCursor();

		}
		if (fbOpen && saveSSfileDialog.HasSelected() && this->saveSS) {

			debug(std::string(saveSSfileDialog.GetPwd().string()));
			debug("SaveSS : " + saveSSfileDialog.GetSelected().filename().string());
			this->saveSpritesheet(saveSSfileDialog.GetPwd().string() + "\\", saveSSfileDialog.GetSelected().filename().string());
			fbOpen = false;
			saveSS = false;
		}






	}

	ImGui::SFML::Render(*m_window);



	m_renderer->swapBuffer();

	return true;


	/***************************************************************************************************/

	if (!m_window->isOpen()) return false;

	//debug("Time = " + std::to_string(fElapsedTime));
	im->update();
	m_camera->update(pos, fElapsedTime);
	m_camera->render(m_window);


	ImGui::SFML::Update(*m_window, m_dt.restart());

	m_renderer->clearWindow();
	m_renderer->drawSprite(pos, "dirt0", 4.0f);
	std::string txt = "Cam Center : " + std::to_string(m_camera->getCenter().x) + ", " + std::to_string(m_camera->getCenter().y);
	std::string txt2 = "Pos : " + std::to_string(pos.x) + ", " + std::to_string(pos.y);
	m_renderer->drawTextRow(this->m_useFont, 0, txt, 0.5f);
	m_renderer->drawTextRow(this->m_useFont, 1, txt2, 0.5f);


	sf::Color bgColor;
	float color[3] = { 0.f, 0.f, 0.f };
	ImGui::Begin("Sample window"); // begin window

									   // Background color edit
	if (ImGui::ColorEdit3("Background color", color)) {
		// this code gets called if color value changes, so
		// the background color is upgraded automatically!
		bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
		bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
		bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
	}


	ImGui::End(); // end window
	ImGui::SFML::Render(*m_window);



	m_renderer->swapBuffer();



	return true;

}
bool sheetEditor::destroy() {
	debug("Cleaning up!");

	if (m_camera != nullptr) {
		delete m_camera;
		m_camera = nullptr;
	}

	if (m_renderer != nullptr) {
		delete m_renderer;
		m_renderer = nullptr;
	}

	if (m_window != nullptr) {
		if (m_window->isOpen())
			m_window->close();
		delete m_window;
		m_window = nullptr;
	}

	return true;
}

std::map<Vector2i, std::string> populateSpriteList(std::string name) {
	std::map<Vector2i, std::string> retval;
	dorkestSpriteMan* dsm = dorkestSpriteMan::getInstance();
	dorkestSpriteSheet* sheet = dsm->getSheet(name);
	json data = sheet->getData();
	json o = data[SPRITEDATA];
	for (json::iterator it = o.begin(); it != o.end(); ++it) {

		debug("Found sprite at " + it.key());
		std::string n = it.key();
		int x = o[n][SPRITELOCX];
		int y = o[n][SPRITELOCY];
		retval[Vector2i(x, y)] = it.key();
	}

	for (auto it = retval.begin(); it != retval.end(); ++it) {
		debug("retval[" + std::to_string(it->first.x) + ", " + std::to_string(it->first.y) + "] = " + it->second);
	}

	return retval;
}

bool sheetEditor::newSpritesheet(std::string path, std::string filename) {
	debug("");
	//Add our spritesheet...
	std::string newname;
	newname = filename;
	newname.resize(newname.size() - 4, 0);


	dsm->addNewSpriteSheet(newname, path + filename, this->sheetDataWin->sX, this->sheetDataWin->sY, 1);
	debug("Created new sprite " + newname);
	activeSS = dsm->getSheet(newname);
	if (activeSS == nullptr) { error("activeSS WAS NOT LOADED!"); }
	activeSaved = false;
	m_camera->setMapSize(activeSS->getData()[SOURCEWIDTH], activeSS->getData()[SOURCEHEIGHT]);

	return true;
}
bool sheetEditor::saveSpritesheet(std::string path, std::string name) {
	debug("");
	activeSS->setSavePath(path + name);
	activeSS->save();
	activeSaved = true;
	return true;
}
bool sheetEditor::loadSpritesheet(std::string path, std::string name) {
	debug("loadSS");
	std::string sheetname = dsm->loadSpriteSheet(path, name);
	if (sheetname == "") {
		std::string sheetname = dsm->replaceSpriteSheet(path, name);
		if (sheetname == "") {
			error("Invalid sheet.");
			return false;
		}
	}

	//populate Active Spritesheet variable.
	activeSS = dsm->getSheet(sheetname);
	//reset cell x and y
	cellx = 0;
	celly = 0;
	// have the sheet register its sprites with the manager.
	activeSS->registerSprites();
	//repopulate our own index of sprites.
	//this->spriteDatas.clear();
	//this->spriteDatas = populateSpriteList(activeSS->getName());
	this->updateSpriteWin();
	activeSaved = true;
	return true;
}

void sheetEditor::updateSpriteWin() {
	int posx = (cellx * activeSS->getData()[SPRITESIZEW]) + (activeSS->getData()[SOURCECELLPADDING] * cellx);
	int posy = (celly * activeSS->getData()[SPRITESIZEH]) + (activeSS->getData()[SOURCECELLPADDING] * celly);

	debug("Trying to find " + Vector2i(posx, posy).toStr());
	std::string name = activeSS->getSpriteAt(cellx, celly);
	if (name != ""){
		
		debug("There is sprite data here : " + name);
		spriteWin->setName(name);
		spriteWin->c[0] = activeSS->getData()[SPRITEDATA][name][SPRITETINTR];
		spriteWin->c[1] = activeSS->getData()[SPRITEDATA][name][SPRITETINTG];
		spriteWin->c[2] = activeSS->getData()[SPRITEDATA][name][SPRITETINTB];
		spriteWin->c[3] = activeSS->getData()[SPRITEDATA][name][SPRITETINTA];
		spriteWin->scale = activeSS->getData()[SPRITEDATA][name][SPRITESCALE];
		spriteWin->setAltGroup(activeSS->getData()[SPRITEDATA][name][SPRITEALTGROUP]);
		spriteWin->setIs3x3(activeSS->getData()[SPRITEDATA][name][IS3X3]);
		spriteWin->set3x3GroupName(activeSS->getData()[SPRITEDATA][name][GROUPNAME3X3]);

	}
	else {
		debug("There is no sprite data.");
		spriteWin->clear();
	}
	this->spriteWin->locx = cellx;
	this->spriteWin->locy = celly;

}

void sheetEditor::cursorMoved() {

	std::string name = this->spriteWin->getName();
	if (name != "") {
		debug("Got string : " + name);
		dorkestSprite* sdef = new dorkestSprite(name, activeSS->getName());
		
		int posx = (cellx * activeSS->getData()[SPRITESIZEW]) + (activeSS->getData()[SOURCECELLPADDING]  * cellx);
		int posy = (celly * activeSS->getData()[SPRITESIZEH]) + (activeSS->getData()[SOURCECELLPADDING]  * celly);
		debug("key = " + Vector2i(posx, posy).toStr());
		sdef->setSpriteCoord(Vector2i(posx,posy));
		sdef->setScaleFactor(spriteWin->scale);
		sdef->setSpriteTint(Colorf(spriteWin->c[0], spriteWin->c[1], spriteWin->c[2], spriteWin->c[3]));
		sdef->setAltGroup(spriteWin->getAltGroup());
		sdef->set3x3GroupName(spriteWin->get3x3GroupName());
		sdef->setIs3x3(spriteWin->getIs3x3());

		activeSS->defineSprite(name, sdef);
		spriteWin->clear();
	}
	else {
		debug("sprite data input didn't change?");
	}
}
