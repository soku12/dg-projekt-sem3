#include "manager.h"

manager_t::State manager_t::runState(manager_t::State wantedState) {
	return (this->*fnctab[wantedState])();
}

manager_t::State manager_t::run_Main() {
	updateLoadedWordlistsString();
	for (int i = 0; i <= button_Unload; i++)
		uitab[i]->setVisibleFlag(true);
	
	while (1) {
		while (window->pollEvent(*event)) {
			if (event->type == sf::Event::MouseButtonReleased) {
				if (event->mouseButton.button == sf::Mouse::Button::Left) {
					if (uitab[button_Exit]->isActive()) {
						uitab.hideAll();
						return Exit;
					}
					else if (uitab[button_Load]->isActive())
						return Load;
					else if (uitab[button_Unload]->isActive())
						return Unload;
				}
			}
		}
	}
}

manager_t::State manager_t::run_Load() {
	uitab[input_InputField]->setVisibleFlag(true);
	std::string filename = uitab[input_InputField]->getString();
	if (filename.empty()) {
		uitab.hideAll();
		return Main;
	}
	for (int i = 0; i < wordlistTab->size(); i++) {
		if ((*wordlistTab)[i].getFilename() == filename) {
			(*wordlistTab)[i].setLoadedFlag(true);
			uitab.hideAll();
			return Main;
		}
	}
	wordlistTab->push_back({ filename });
	return Main;
}

manager_t::State manager_t::run_Unload() {
	uitab[input_InputField]->setVisibleFlag(true);
	std::string filename = uitab[input_InputField]->getString();
	if (filename.empty()) {
		uitab.hideAll();
		return Main;
	}
	for (int i = 0; i < wordlistTab->size(); i++) {
		if ((*wordlistTab)[i].getFilename() == filename) {
			(*wordlistTab)[i].setLoadedFlag(false);
		}
	}
	return Main;
}

void manager_t::updateLoadedWordlistsString() {
	std::string str = "Loaded wordlists:\n";
	for (int i = 0; i < wordlistTab->size(); i++) {
		if ((*wordlistTab)[i].isLoaded() && !(*wordlistTab)[i].getErrorFlag()) {
			str += (*wordlistTab)[i].getFilename() + "\n";
		}
	}
	uitab[text_LoadedWordlists]->setString(str);
}

manager_t::manager_t(sf::RenderWindow * window_, sf::Event * event_, vector<wordlist_t>* wordlistTab_, font_tab_t * fonts_)
	:window{ window_ }, event{ event_ }, wordlistTab{ wordlistTab_ }, fonts{ fonts_ } {

	uitab.registerElem(new text_t(50, 60, false, window, event, "WORDLIST MANAGER", sf::Color::White, sf::Color::Black, 36, fonts->SegoeUI, 2));
	uitab.registerElem(new text_t(400, 150, false, window, event, "Loaded wordlists:\n", sf::Color::White, sf::Color::Black, 16, fonts->SegoeUI, 2));
	uitab.registerElem(new button_t("", 0, 0, false, window, event, "Exit", sf::Color::White, sf::Color::Black, 16, fonts->SegoeUI, 2, 60, 30, sf::Color::Red, sf::Color::Black));
	uitab.registerElem(new button_t("", 100, 200, false, window, event, "Load", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 150, 40, sf::Color::Green, sf::Color::Black));
	uitab.registerElem(new button_t("", 100, 260, false, window, event, "Unload", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 150, 40, sf::Color::Cyan, sf::Color::Black));
	uitab.registerElem(new text_input_field_t(100, 400, false, window, event, "", sf::Color::White, sf::Color::Black, 20, fonts->SegoeUI, 2, 250, 30));
	uitab.registerElem(new text_t(100, 420, false, window, event, "Enter wordlist filename", sf::Color::White, sf::Color::Black, 16, fonts->SegoeUI, 2));

	currentState = Main;
	fnctab[Main] = &manager_t::run_Main;
	fnctab[Load] = &manager_t::run_Load;
	fnctab[Unload] = &manager_t::run_Unload;
}

manager_t::manager_t(const manager_t & copy)
	:window{ copy.window }, event{ copy.event }, wordlistTab{ copy.wordlistTab }, fonts{ copy.fonts }, uitab{ copy.uitab } {
	fnctab[Main] = &manager_t::run_Main;
}

void manager_t::run() {
	while (currentState != Exit)
		currentState = runState(currentState);
	currentState = Main;
}

void manager_t::drawUI() {
	for (int i = 0; i < UIID_COUNT; i++) {
		if (uitab[i]->isVisible())
			uitab[i]->draw();
	}
}
