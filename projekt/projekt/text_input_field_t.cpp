#include "text_input_field_t.h"

text_input_field_t::text_input_field_t(int posX_, int posY_, bool visibleFlag_, sf::RenderWindow * window, sf::Event* ev_, std::string txt_, sf::Color txtFillColor_, sf::Color txtOutlineColor_, int txtSize_, sf::Font& txtFont_, int txtOutlineThickness_, int sizeX_, int sizeY_) 
	:text_t{ posX_, posY_, visibleFlag_, window, ev_, txt_, txtFillColor_, txtOutlineColor_, txtSize_, txtFont_, txtOutlineThickness_ } {
	fieldShape.setPosition(posX_, posY_);
	fieldShape.setFillColor(sf::Color::White);
	fieldShape.setOutlineColor(sf::Color::Black);
	fieldShape.setOutlineThickness(1);
	sf::Vector2f tmpSize(sizeX_, sizeY_);
	fieldShape.setSize(tmpSize);
}

std::string text_input_field_t::getString() {
	clear();
	setVisibleFlag(true);
	while (1) {
		while (window->pollEvent(*event)) {
			if (event->type == sf::Event::TextEntered) {
				if (event->text.unicode == '\b') {
					if (input.size() > 0)
						input.erase(input.size() - 1, 1);
				}
				else
					input += event->text.unicode;
				text_t::setString(input);
			}
			else if (event->type == sf::Event::KeyPressed) {
				if (event->key.code == sf::Keyboard::Enter) {
					setVisibleFlag(false);
					return input;
				}
				else if (event->key.code == sf::Keyboard::Escape) {
					setVisibleFlag(false);
					return "";
				}
			}
		}
	}
	return "error";
}

void text_input_field_t::draw() {
	window->draw(fieldShape);
	text_t::draw();
}

void text_input_field_t::clear() {
	input = "";
	text_t::setString(input);
}

