#include "button.h"

//witamy w kolonii
button_t::button_t(std::string input_, int posX_, int posY_, bool visibleFlag_, sf::RenderWindow * window_, sf::Event* ev_,  std::string txt_, sf::Color txtFillColor_, sf::Color txtOutlineColor_, 
	int txtSize_, sf::Font& txtFont_, int txtOutlineThickness_, int sizeX_, int sizeY_, sf::Color buttonColor_, sf::Color activeOutlineColor_)
	:text_t{ posX_ + 10, posY_ + CENTER(sizeY_, txtSize_), visibleFlag_, window_, ev_, txt_, txtFillColor_, txtOutlineColor_, txtSize_, txtFont_, txtOutlineThickness_ },
	input{ input_ }, TLcornerX{ posX_ }, TLcornerY{ posY_ }, BRcornerX{ posX_ + sizeX_ }, BRcornerY{ posY_ + sizeY_ } {
	
	buttonShape.setPosition(posX_, posY_);
	buttonShape.setFillColor(buttonColor_);
	buttonShape.setOutlineThickness(-1);
	buttonShape.setOutlineColor(sf::Color::Black);

	activeOutlineShape.setPosition(posX_ + 2, posY_ + 2);
	activeOutlineShape.setOutlineThickness(-3);
	activeOutlineShape.setOutlineColor(activeOutlineColor_);
	activeOutlineShape.setFillColor(sf::Color::Transparent);

	sf::Vector2f sizetmp(sizeX_, sizeY_);
	buttonShape.setSize(sizetmp);
	sizetmp.x = sizeX_ - 4;
	sizetmp.y = sizeY_ - 4;
	activeOutlineShape.setSize(sizetmp);

}

bool button_t::isActive() {
	sf::Vector2i mousepos = sf::Mouse::getPosition(*window);
	if (mousepos.x >= TLcornerX &&
		mousepos.x <= BRcornerX && 
		mousepos.y >= TLcornerY &&
		mousepos.y <= BRcornerY &&
		visibleFlag == true) {
		return true;
	}
	return false;
}

void button_t::draw() {
	window->draw(buttonShape);
	text_t::draw();
	if (this->isActive()) {
		window->draw(activeOutlineShape);
	}
}
