#pragma once
#include "ui_elem.h"

class text_t :public ui_elem_t {
protected:
	sf::Font font;
	sf::Text text;
public:
	text_t() = default;
	text_t(int posX_, int posY_, bool visibleFlag_, sf::RenderWindow* window, sf::Event* ev_, std::string txt_, sf::Color fillColor_, sf::Color outlineColor_, int size_, sf::Font& font_, int outlineThickness_ );
	text_t(const text_t&) = default;
	~text_t() = default;

	void operator=(const text_t& right);

	virtual void draw() { window->draw(text); }
	virtual bool isActive() { return false; }
	virtual std::string getString();
	virtual void setString(std::string string_) { text.setString(string_); }
};

