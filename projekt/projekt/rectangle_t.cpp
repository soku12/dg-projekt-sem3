#include "rectangle_t.h"

rectangle_t::rectangle_t(int posX_, int posY_, bool visibleFlag_, sf::RenderWindow * window_, sf::Event * event_, int sizeX_, int sizeY_, int rotation_, sf::Color fillColor_, sf::Color outlineColor_, int outlineThickness_)
	:ui_elem_t{posX_, posY_, visibleFlag_, window_, event_} {
	shape.setPosition(posX_, posY_);
	sf::Vector2f size(sizeX_, sizeY_);
	shape.setSize(size);
	shape.setOrigin(sizeX_ / 2, sizeY_ / 2);
	shape.setRotation(rotation_);
	shape.setFillColor(fillColor_);
	shape.setOutlineColor(outlineColor_);
	shape.setOutlineThickness(outlineThickness_);
}

rectangle_t::rectangle_t(const rectangle_t & copy)
	:ui_elem_t{ copy.posX, copy.posY, copy.visibleFlag, copy.window, copy.event },
	shape{ copy.shape } {}

