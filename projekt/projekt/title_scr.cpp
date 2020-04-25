#include "title_scr.h"

title_t::State title_t::runState(State wantedState) {
	return (this->*fnctab[wantedState])();
}

title_t::State title_t::run_Main() {
	uitab[text_Title]->setVisibleFlag(true);
	uitab[button_Game]->setVisibleFlag(true);
	uitab[button_Stats]->setVisibleFlag(true);
	uitab[button_Manager]->setVisibleFlag(true);
	uitab[button_Exit]->setVisibleFlag(true);

	while (1) {
		while (window->pollEvent(*event)) {
			if (event->type == sf::Event::MouseButtonReleased) {
				if (event->mouseButton.button == sf::Mouse::Left) {
					if (uitab[button_Game]->isActive())
						return Game;
					else if (uitab[button_Stats]->isActive())
						return Stats;
					else if (uitab[button_Manager]->isActive())
						return Manager;
					else if (uitab[button_Exit]->isActive())
						return Exit;
				}
			}
		}
	}
}

title_t::State title_t::run_Game() {
	uitab.hideAll();
	game->run();
	return Main;
}

title_t::State title_t::run_Manager() {
	uitab.hideAll();
	manager->run();
	return Main;
}

title_t::State title_t::run_Stats() {
	uitab.hideAll();
	stats->run();
	return Main;
}

title_t::title_t(sf::RenderWindow* window_, sf::Event* event_, game_t* game_, stats_t* stats_, manager_t* manager_, font_tab_t* fonts_) 
	:window{ window_ }, event{ event_ }, game{ game_ }, stats{ stats_ }, manager{ manager_ }, fonts{ fonts_ } {

	uitab.registerElem(new text_t(50, 50, true, window, event, "HANGMAN", sf::Color::White, sf::Color::Black, 40, fonts->SegoeUI, 3));
	uitab.registerElem(new button_t("", 100, 200, true, window, event, "PLAY", sf::Color::White, sf::Color::Black, 28, fonts->SegoeUI, 2, 150, 40, sf::Color::Green, sf::Color::Red));
	uitab.registerElem(new button_t("", 100, 260, true, window, event, "Stats", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 150, 40, sf::Color::Cyan, sf::Color::Red));
	uitab.registerElem(new button_t("", 100, 320, true, window, event, "Wordlist manager", sf::Color::White, sf::Color::Black, 16, fonts->SegoeUI, 2, 150, 40, sf::Color(127, 0, 255, 255), sf::Color::Black));
	uitab.registerElem(new button_t("", 100, 380, true, window, event, "Quit", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 150, 40, sf::Color::Red, sf::Color::Black));

	currentState = Main;
	fnctab[Main] = &title_t::run_Main;
	fnctab[Game] = &title_t::run_Game;
	fnctab[Manager] = &title_t::run_Manager;
	fnctab[Stats] = &title_t::run_Stats;
}

void title_t::run() {
	while (currentState != Exit) {
		currentState = runState(currentState);
	}
	currentState = Main;
}

void title_t::drawUI() {
	for (int i = 0; i < UIID_COUNT; i++) {
		if (uitab[i]->isVisible())
			uitab[i]->draw();
	}
}

