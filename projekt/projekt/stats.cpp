#include "stats.h"


stats_t::stats_t(sf::RenderWindow * window_, sf::Event * event_, font_tab_t * fonts_)
	:window{ window_ }, event{ event_ }, fonts{ fonts_ }, 
	gameTimeSeconds{ 0 }, gamesPlayed{ 0 }, gamesWon{ 0 }, winRatio{ 0 } {
	currentState = Main;
	fnctab[0] = &stats_t::run_Main;

	uitab.registerElem(new text_t(100, 100, false, window, event, "STATS", sf::Color::White, sf::Color::Black, 40, fonts->SegoeUI, 2));
	uitab.registerElem(new button_t("", 0, 0, false, window, event, "Exit", sf::Color::White, sf::Color::Black, 16, fonts->SegoeUI, 2, 60, 30, sf::Color::Red, sf::Color::Black));
	uitab.registerElem(new text_t(100, 200, false, window, event, "Games won:\t", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2));
	uitab.registerElem(new text_t(100, 240, false, window, event, "Games lost:\t", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2));
	uitab.registerElem(new text_t(100, 280, false, window, event, "Total games played:\t", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2));
	uitab.registerElem(new text_t(100, 320, false, window, event, "Win ratio:\t", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2));
	uitab.registerElem(new text_t(100, 360, false, window, event, "Time played:\t", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2));

	std::ifstream file("stats");
	std::string line;
	int lineNumber = 0;
	if (file) {
		while (std::getline(file, line)) {
			std::stringstream ss;
			lineNumber++;
			ss << line;
			if (lineNumber % 3 == 1)
				ss >> gamesPlayed;
			else if (lineNumber % 3 == 2)
				ss >> gamesWon;
			else if (lineNumber % 3 == 0)
				ss >> gameTimeSeconds;
		}
	}
}

stats_t::~stats_t() {
	std::ofstream file("stats");
	if (file) {
		file << gamesPlayed << std::endl << gamesWon << std::endl << gameTimeSeconds;
		file.close();
	}
}

stats_t::State stats_t::run_Main() {
	if (gamesPlayed > 0) {
		double gw = gamesWon;
		double gp = gamesPlayed;
		winRatio = gw / gp * 100;
	}

	uitab[text_GamesWon]->setString("Games won:\t" + std::to_string(gamesWon));
	uitab[text_GamesLost]->setString("Games lost:\t" + std::to_string(gamesPlayed - gamesWon));
	uitab[text_TotalGames]->setString("Total games played:\t" + std::to_string(gamesPlayed));
	uitab[text_WinRatio]->setString("Win ratio:\t" + std::to_string(winRatio) + "%");
	uitab[text_TimePlayed]->setString("Time played:\t" + getTimeString());

	for (int i = 0; i < UIID_COUNT; i++)
		uitab[i]->setVisibleFlag(true);

	while (1) {
		while (window->pollEvent(*event)) {
			if (event->type == sf::Event::MouseButtonReleased) {
				if (event->mouseButton.button == sf::Mouse::Button::Left) {
					if (uitab[button_Exit]->isActive()) {
						uitab.hideAll();
						return Exit;
					}
				}
			}
		}
	}
}

stats_t::State stats_t::runState(stats_t::State wantedState) {
	return (this->*fnctab[wantedState])();
}

void stats_t::run() {
	while (currentState != Exit)
		currentState = runState(currentState);

	currentState = Main;
}

void stats_t::drawUI() {
	for (int i = 0; i < UIID_COUNT; i++) {
		if (uitab[i]->isVisible())
			uitab[i]->draw();
	}
}

void stats_t::addVictory() {
	gamesPlayed += 1;
	gamesWon += 1;
	winRatio = gamesWon / gamesPlayed;
}

void stats_t::addDefeat() {
	gamesPlayed += 1;
	winRatio = gamesWon / gamesPlayed;
}

std::string stats_t::getTimeString() {
	int seconds = gameTimeSeconds;
	int minutes = seconds / 60;
	int hours = minutes / 60;

	minutes = minutes % 60;
	seconds = seconds % 60;

	return std::to_string(hours) + "h " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s";
}

