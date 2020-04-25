#pragma once
#include "text_t.h"

class text_input_field_t :public text_t {
	std::string input;
	sf::RectangleShape fieldShape;	
	void clear();
public:
	text_input_field_t(int posX_, int posY_, bool visibleFlag_, sf::RenderWindow* window, sf::Event* ev_, std::string txt_, sf::Color txtFillColor_, sf::Color txtOutlineColor_, int txtSize_, sf::Font& txtFont_, int txtOutlineThickness_, int sizeX_, int sizeY_);
	~text_input_field_t() = default;
	virtual std::string getString();
	virtual void draw();
	virtual bool isActive() { return false; }
	virtual void setString(std::string string_) { text.setString(string_); }

};

