#include "engine.h"

engine_t::engine_t(vector<std::string> argvString)
	:window{ sf::VideoMode(WINDOW_W, WINDOW_H), "test", sf::Style::Titlebar }, exitFlag{ 0 },
	manager{ &window, &event, &wordlistTab, &fonts },
	stats{ &window, &event, &fonts },
	game{ &window, &event, &wordlistTab, &fonts, &stats },
	title{ &window, &event, &game, &stats, &manager, &fonts } {

	window.setFramerateLimit(60);

	if (argvString.size() > 1) {
		for (int i = 1; i < argvString.size(); i++) {
			wordlistTab.push_back({ argvString[i] });
		}
	}
	
}

void engine_t::start() {
	title.run();
	setExitFlag(true);
}

void engine_t::drawUI() {
	title.drawUI();
	game.drawUI();
	manager.drawUI();
	stats.drawUI();
}
