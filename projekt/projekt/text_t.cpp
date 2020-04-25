#include "text_t.h"

text_t::text_t(int posX_, int posY_, bool visibleFlag_, sf::RenderWindow * window, sf::Event* ev_, std::string txt_, sf::Color fillColor_, sf::Color outlineColor_, int size_, sf::Font& font_, int outlineThickness_)
	:ui_elem_t{ posX_, posY_, visibleFlag_, window, ev_ } {
	text.setString(txt_);
	text.setFillColor(fillColor_);
	text.setOutlineColor(outlineColor_);
	text.setCharacterSize(size_);
	text.setFont(font_);
	text.setOutlineThickness(outlineThickness_);
	text.setPosition(posX_, posY_);
}

void text_t::operator=(const text_t & right) {
	posX = right.posX;
	posY = right.posY;
	visibleFlag = right.visibleFlag;
	window = right.window;
	event = right.event;

	font = right.font;
	text = right.text;
}

std::string text_t::getString() {
	std::string str = text.getString();
	return str;
}
