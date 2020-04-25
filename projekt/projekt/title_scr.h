#pragma once
#include "vector.h"
#include "ui_elem.h"
#include "ui_elem_tab_t.h"
#include "button.h"
#include "text_input_field_t.h"
#include "game.h"
#include "font_tab_t.h"
#include "manager.h"

class title_t {
	enum State { Main, Game, Manager, Stats, Exit, STATE_COUNT };
	enum UIID { text_Title, button_Game, button_Stats, button_Manager, button_Exit, UIID_COUNT };
	
	sf::Font test;

	ui_elem_tab_t uitab;
	sf::Event* event;
	sf::RenderWindow* window;
	font_tab_t* fonts;

	game_t* game;
	stats_t* stats;
	manager_t* manager;

	State currentState;
	State(title_t::*fnctab[STATE_COUNT])();

	State runState(State wantedState);
	State run_Main();
	State run_Game();
	State run_Manager();
	State run_Stats();
public:
	title_t(sf::RenderWindow* window_, sf::Event* event, game_t* game_, stats_t* stats_, manager_t* manager_, font_tab_t* fonts_);
	~title_t() = default;
	void run();
	void drawUI();
};

