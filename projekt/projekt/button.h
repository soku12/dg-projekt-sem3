#pragma once
#include "text_t.h"
#include "macros.h"
#include <string>
#include <SFML\Graphics.hpp>

class button_t : public text_t {
private:
	// coords of the button corners (bounds)
	int TLcornerX; // top left corner X
	int TLcornerY; // top left corner Y
	int BRcornerX; // bottom right corner X
	int BRcornerY; // bottom right corner Y

	sf::Color buttonColor;
	sf::Color activeOutlineColor;
	sf::RectangleShape buttonShape;
	sf::RectangleShape activeOutlineShape;

	std::string input;

public:
	button_t() = default;
	button_t(std::string input, int posX_, int posY_, bool visibleFlag_, sf::RenderWindow* window_, sf::Event* ev_, std::string txt_, sf::Color txtFillColor_, sf::Color txtOutlineColor_, 
		int txtSize_, sf::Font& txtFont_, int txtOutlineThickness_, int sizeX_, int sizeY_, sf::Color buttonColor_, sf::Color activeOutlineColor_);
	button_t(const button_t&) = default;
	~button_t() = default;

	// returns true if the mouse is within the bounds of the button
	virtual bool isActive();
	virtual void draw();
	virtual std::string getString() { return input; }
	virtual void setString(std::string string_) { text.setString(string_); }
};

