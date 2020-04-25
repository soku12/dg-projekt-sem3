#include "circle_t.h"

circle_t::circle_t(int posX_, int posY_, bool visibleFlag_, sf::RenderWindow * window_, sf::Event * event_, int radius_, sf::Color fillColor_, sf::Color outlineColor_, int outlineThickness_)
	:ui_elem_t{posX_,posY_, visibleFlag_, window_, event_} {
	shape.setPosition(posX_, posY_);
	shape.setRadius(radius_);
	shape.setFillColor(fillColor_);
	shape.setOutlineColor(outlineColor_);
	shape.setOutlineThickness(outlineThickness_);
}

circle_t::circle_t(const circle_t & copy)
	:ui_elem_t{ copy.posX, copy.posY, copy.visibleFlag, copy.window, copy.event },
	shape{ copy.shape } {}
