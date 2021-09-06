#include <Engine/inputManager.h>
#include <imgui-SFML.h>

inputMan* inputMan::m_instance = 0;

inputMan::inputMan() {
	debug("Creating new InputEvent bus.")
		this->m_bus = new EventBus();
	this->m_window = nullptr;
}

bool inputMan::update() {
	sf::Event event;
	
	// while there are pending events...
	while (this->m_window->pollEvent(event)) {
		switch (event.type)
		{
		case sf::Event::Closed:
			this->m_bus->publish<windowEvent>(new windowEvent(winEvent::CLOSING));
			break;
		case sf::Event::Resized:
			this->m_bus->publish<windowEvent>(new windowEvent(winEvent::RESIZE, Vector2i(event.size.width,event.size.height)));
			break;
		case sf::Event::LostFocus:
			this->m_bus->publish<windowEvent>(new windowEvent(winEvent::LOSTFOCUS));
			break;
		case sf::Event::GainedFocus:
			this->m_bus->publish<windowEvent>(new windowEvent(winEvent::GAINEDFOCUS));
			break;
		case sf::Event::KeyPressed:
			this->m_bus->publish<keyEvent>(new keyEvent(event.key, true));
			break;
		case sf::Event::KeyReleased:
			this->m_bus->publish<keyEvent>(new keyEvent(event.key, false));
			break;
		case sf::Event::MouseWheelScrolled:
			this->m_bus->publish<mouseEvent>(new mouseEvent(mouseEventType::WHEEL, 0, Vector2i(0, 0), event.mouseWheel.delta));
			break;
		case sf::Event::MouseButtonPressed:
			this->m_bus->publish<mouseEvent>(new mouseEvent(mouseEventType::BUTTONDOWN, event.mouseButton.button + 1, Vector2i(event.mouseButton.x, event.mouseButton.y), 0));
			break;
		case sf::Event::MouseButtonReleased:
			this->m_bus->publish<mouseEvent>(new mouseEvent(mouseEventType::BUTTONUP, event.mouseButton.button + 1, Vector2i(event.mouseButton.x, event.mouseButton.y), 0));
			break;
		case sf::Event::MouseMoved:
			this->m_bus->publish<mouseEvent>(new mouseEvent(mouseEventType::MOVE, 0, Vector2i(event.mouseMove.x, event.mouseMove.y), 0));
			break;
		case sf::Event::MouseLeft:
			this->m_bus->publish<mouseEvent>(new mouseEvent(mouseEventType::EXIT, 0, Vector2i(0, 0), 0));
			m_mouseInWindow = false;
			break;
		case sf::Event::MouseEntered:
			this->m_bus->publish<mouseEvent>(new mouseEvent(mouseEventType::ENTER, 0, Vector2i(0, 0), 0));
			m_mouseInWindow = true;
			break;
		}
		ImGui::SFML::ProcessEvent(event);
	}

	return true;
}

void inputMan::shutdown() {
	if (inputMan::m_instance != nullptr) {
		debug("Shutting down input manager.");
		m_bus->publish<inputShutdownEvent>(new inputShutdownEvent());
		delete inputMan::m_instance;
		inputMan::m_instance = nullptr;
	}
}

inputMan::~inputMan() {
	if (inputMan::m_instance != nullptr)
		inputMan::m_instance->shutdown();


}

inputMan* inputMan::getInstance() {
	if (inputMan::m_instance == 0) {
		inputMan::m_instance = new inputMan();
	}

	return inputMan::m_instance;
}

EventBus* inputMan::getBus() {
	return this->m_bus;
}

void inputMan::setWindow(sf::RenderWindow* win) {

	inputMan::getInstance()->m_window = win;

}
