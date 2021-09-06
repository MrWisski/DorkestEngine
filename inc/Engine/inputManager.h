#pragma once
#include <Util/EventSystem.h>
#include <SFML/Graphics.hpp>
#include <Util/Math/Vector2.h>

enum class mouseEventType { WHEEL, BUTTONDOWN, BUTTONUP, MOVE, ENTER, EXIT };

enum class winEvent { CLOSING, RESIZE, LOSTFOCUS, GAINEDFOCUS };


class keyEvent : public Event {
public:
	keyEvent(sf::Event::KeyEvent e, bool action) {
		key = e.code;
		this->action = action;
		modAlt = e.alt;
		modShift = e.shift;
		modSystem = e.system;
	}


	int key, action;
	bool modAlt, modShift, modSystem;

};

class mouseEvent : public Event {
public:
	mouseEvent(mouseEventType t, int button,Vector2i pos, float delta) {
		eType = t;
		this->button = button;
		this->pos = pos;
		this->delta = delta;
	}

	mouseEventType eType;
	int button;
	Vector2i pos;
	float delta;

};



class windowEvent : public Event {
public:

	windowEvent(winEvent e) : eType(e) { newSize = Vector2i(0, 0); }
	windowEvent(winEvent e, Vector2i size) : eType(e),newSize(size) {}


	winEvent eType;
	Vector2i newSize;
};

class inputShutdownEvent : public Event {
public:
	inputShutdownEvent() {}
};


class inputMan {
public:

	static inputMan* getInstance();

	EventBus* getBus();
	bool update();
	void shutdown();
	void setWindow(sf::RenderWindow* win);
private:
	inputMan();
	~inputMan();

	static inputMan* m_instance;
	EventBus* m_bus = nullptr;
	sf::RenderWindow* m_window;
	bool m_mouseInWindow = true;
};
