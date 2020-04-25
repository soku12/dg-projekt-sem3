#pragma once
#include <SFML/Graphics.hpp>

class engine_t;

class ui_elem_t {
protected:
	int posX;
	int posY;
	bool visibleFlag;
	sf::RenderWindow* window;
	sf::Event* event;

public:
	ui_elem_t() = default;
	ui_elem_t(int x_, int y_, bool vis_, sf::RenderWindow* win_, sf::Event* ev_) 
		:posX{ x_ }, posY{ y_ }, visibleFlag{ vis_ }, window{ win_ }, event{ ev_ } {}
	ui_elem_t(const ui_elem_t&) = default;
	~ui_elem_t() = default;

	bool isVisible() { return visibleFlag; }
	void setVisibleFlag (bool val) { visibleFlag = val; }

	virtual void draw() = 0;
	virtual bool isActive() = 0;
	virtual std::string getString() = 0;
	virtual void setString(std::string string_) = 0;
};

