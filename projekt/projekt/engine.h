#pragma once
#include "wordlist.h"
#include "game.h"
#include "manager.h"
#include "stats.h"
#include "title_scr.h"
#include "font_tab_t.h"


class engine_t {
	bool exitFlag;
public:
	sf::RenderWindow window;
	sf::Event event;
	font_tab_t fonts;

	vector<wordlist_t> wordlistTab;
	title_t title;
	game_t game;
	manager_t manager;
	stats_t stats;

	

	engine_t(vector<std::string> argvString);
	~engine_t() = default;
	void start();
	void drawUI();
	void setExitFlag(bool val) { exitFlag = val; }
	bool getExitFlag() { return exitFlag; }
};

