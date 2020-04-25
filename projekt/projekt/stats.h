#pragma once
#include "ui_elem.h"
#include "ui_elem_tab_t.h"
#include "button.h"
#include "text_t.h"
#include "font_tab_t.h"
#include <vector>
#include <fstream>
#include <sstream>




class stats_t {
	enum State{Main, Exit, STATE_COUNT};
	enum UIID{text_Title, button_Exit, text_GamesWon, text_GamesLost, text_TotalGames, text_WinRatio, text_TimePlayed, UIID_COUNT};

	ui_elem_tab_t uitab;
	sf::RenderWindow* window;
	sf::Event* event;
	font_tab_t* fonts;

	double gameTimeSeconds;
	unsigned int gamesPlayed;
	unsigned int gamesWon;
	float winRatio;

	State currentState;
	State(stats_t::*fnctab[STATE_COUNT])();
	
	State runState(State state);
	State run_Main();
public:
	stats_t() = default;
	stats_t(sf::RenderWindow* window_, sf::Event* event_, font_tab_t* fonts_);
	~stats_t();
	void run();
	void drawUI();
	void addVictory();
	void addDefeat();
	void addGameTime(double time) { gameTimeSeconds += time; }
	std::string getTimeString();
};

