#pragma once
#include <SFML/Graphics.hpp>

class font_tab_t {
public:	
	sf::Font SegoeUI;
	font_tab_t() { SegoeUI.loadFromFile("segoeui.ttf"); }
	~font_tab_t() = default;
};

