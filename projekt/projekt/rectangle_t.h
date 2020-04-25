#pragma once
#include "ui_elem.h"

class rectangle_t :public ui_elem_t {
	sf::RectangleShape shape;
public:
	rectangle_t() = default;
	rectangle_t(int posX_, int posY_, bool visibleFlag_, sf::RenderWindow* window_, sf::Event* event_, int sizeX_, int sizeY_, int rotation_, sf::Color fillColor_, sf::Color outlineColor_, int outlineThickness_);
	rectangle_t(const rectangle_t& copy);
	~rectangle_t() = default;
	virtual void draw() { window->draw(shape); }
	virtual std::string getString() { return ""; }
	virtual bool isActive() { return false; }
	virtual void setString(std::string string_) {};

};

