#define OLC_PGEX_SOUND
#define OLC_PGEX_AUDIOCONVERT

#include <Engine\Engine.h>
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

bool Engine::pressAnyKey() {

	return false;
}
bool Engine::initEngine() {

	if (!initWindow()) { return false; }

	dsm = dorkestSpriteMan::getInstance();
	this->m_camera = new dorkestCamera(1024, 512,0,0);
	this->m_renderer = new dorkestRenderer(m_window, m_camera);
	pos.x = 0;
	pos.y = 0;
	m_camera->setTopDownView();
	m_camera->setMapSize(2048 *2, 2048*2);
	m_camera->setMapCorner(0, 0);

	m_camera->setCenter(512, 256);
	m_camera->render(m_window);
	this->m_useFont = new sf::Font();
	if (!this->m_useFont->loadFromFile("./res/Fonts/Kenney Future.ttf")) {
		error("Error loading font from file!");
		return false;
	}

	debug("Loaded Sprite Sheet : " + dsm->loadSpriteSheet("./res", "roguelike.json"));
	this->m_camera->setScreenSize(1024, 512);
	
	ImGui::SFML::Init(*m_window);

	return true;

}

bool Engine::initWindow() {
	if (dsm != nullptr) {
		error("Call initWindow() before initEngine()!");
		return false;
	}
	m_window = new sf::RenderWindow(sf::VideoMode(1024, 512), "Dorkest Engine", sf::Style::Default);
	if (m_window == nullptr) {
		error("Failed to create window!");
		return false;
	}
	m_window->setVisible(true);

	debug("Setting up window input manager.");
	this->im = inputMan::getInstance();
	im->setWindow(m_window);
	im->getBus()->subscribe(this, &Engine::handleWinEvent);
	im->getBus()->subscribe(this, &Engine::handleKeyEvent);
	im->getBus()->subscribe(this, &Engine::handleMouseEvent);

	return true;

}

void Engine::handleKeyEvent(keyEvent* e) {
	if (!e->action) return;
	debug("got keypress.");
	switch (e->key) {
	case sf::Keyboard::W:
		
		pos.y -= 64;
		if (pos.y < 0) pos.y = 0;
		break;
	case sf::Keyboard::A:
		pos.x -= 64;
		if (pos.x < 0) pos.x = 0;
		break;
	case sf::Keyboard::S:
		pos.y += 64;
		if (pos.y >= m_camera->getMapSize().y-64) pos.y = m_camera->getMapSize().y - 64;
		break;
	case sf::Keyboard::D:
		pos.x += 64;
		if (pos.x >= m_camera->getMapSize().x-64) pos.x = m_camera->getMapSize().x - 64;
		break;
	case sf::Keyboard::Escape:
		break;
	}
}
void Engine::handleMouseEvent(mouseEvent* e) {}

void Engine::handleWinEvent(windowEvent* e) {
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
bool Engine::initSound() {
	return true;
}

bool Engine::update(float fElapsedTime) {
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
bool Engine::destroy() {
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