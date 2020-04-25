#pragma once
#include "ui_elem.h"
class circle_t :public ui_elem_t {
	sf::CircleShape shape;
public:
	circle_t() = default;
	circle_t(int posX_, int posY_, bool visibleFlag_, sf::RenderWindow* window_, sf::Event* event_, int radius_, sf::Color fillColor_, sf::Color outlineColor_, int outlineThickness_);
	circle_t(const circle_t& copy);
	~circle_t() = default;
	virtual void draw() { window->draw(shape); }
	virtual std::string getString() { return ""; }
	virtual bool isActive() { return false; }
	virtual void setString(std::string string_) {};
};

