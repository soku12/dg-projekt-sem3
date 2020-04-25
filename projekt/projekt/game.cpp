#include "game.h"

game_t::State game_t::runState(State wantedState) {
	return (this->*fnctab[wantedState])();
}

game_t::State game_t::run_Start() {
	exitFlag = false;
	//check if a viable wordlist exists
	int validWordlists = 0;
	if (wordlistTab->size() < 1) {
		return Exit;
	}
	else for (int i = 0; i < wordlistTab->size(); i++) {
		if ((*wordlistTab)[i].isValid()) {
			validWordlists++;
		}
	}

	if (validWordlists < 1) {
		return ExitError;
	}
	
	//generating random numbers for random wordlist and word selection
	std::default_random_engine randomotron;
	randomotron.seed(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> wordlistDistr(0, wordlistTab->size() - 1);
	int chosenWordlist = wordlistDistr(randomotron);
	while ((*wordlistTab)[chosenWordlist].isValid() == false) {
		int chosenWordlist = wordlistDistr(randomotron);
	}

	std::uniform_int_distribution<int> wordDist(0, (*wordlistTab)[chosenWordlist].getWordCount() - 1);
	int chosenWord = wordDist(randomotron);

	gameWord = (*wordlistTab)[chosenWordlist][chosenWord];
	createGameText();
	uitab[text_Category]->setString("category: " + gameWord.getCategory());
	startTime = std::chrono::system_clock::now();
	return Main;
}

game_t::State game_t::run_Main() {
	if (getInProgressFlag())
		pauseRecovery();
	else for (int i = 0; i < UIID_COUNT; i++) {
		if (i >= input_Guess && i <= rect_HMRightLeg)
			continue;
		uitab[i]->setVisibleFlag(true);
	}
	
	setInProgressFlag(true);
	while (1) {
		while (window->pollEvent(*event)) {
			if (event->type == sf::Event::MouseButtonReleased) {
				if (event->mouseButton.button == sf::Mouse::Button::Left) {
					if (uitab[button_Exit]->isActive()) {
						return ExitPrompt;
					}
					else if (uitab[button_Guess]->isActive()) {
						std::string guess = uitab[input_Guess]->getString();
						if (guess.empty())
							return Main;
						else if (guess == gameWord.getWord()) {
							wordReveal();
							return ExitWin;
						}
						else {
							wrongTries++;
							uitab[rect_HMBase - 1 + wrongTries]->setVisibleFlag(true);
							if (wrongTries > 9)
								return ExitLose;
						}
					}
					else for (int i = kbbut_Q; i <= kbbut_M; i++) {
						if (uitab[i]->isActive()) {
							char pickedLetter = uitab[i]->getString()[0];
							if (gameWord.checkLetter(pickedLetter)) {
								guessedLetters.push_back(pickedLetter);
								updateGameText(pickedLetter);
								uitab[i]->setVisibleFlag(false);
								if (guessedLetters.size() == gameWord.getUniqueLetterList().size())
									return ExitWin;
							}
							else {
								wrongLetters.push_back(pickedLetter);
								wrongTries++;
								uitab[rect_HMBase - 1 + wrongTries]->setVisibleFlag(true);
								uitab[i]->setVisibleFlag(false);
								if (wrongTries > 9)
									return ExitLose;
							}
						}
					}
				}
			}
		}
	}

}

game_t::State game_t::run_ExitPrompt() {
	pauseSave();
	uitab.hideAll();
	uitab[text_ExitPrompt]->setVisibleFlag(true);
	uitab[button_ExitYES]->setVisibleFlag(true);
	uitab[button_ExitNO]->setVisibleFlag(true);
	while (1) {
		while (window->pollEvent(*event)) {
			if (event->type == sf::Event::MouseButtonReleased) {
				if (event->mouseButton.button == sf::Mouse::Button::Left) {
					if (uitab[button_ExitYES]->isActive()) {
						exitFlag = true;
						return ExitLose;
					}
					else if (uitab[button_ExitNO]->isActive()) {
						return Main;
					}
				}
			}
		}
	}
}

game_t::State game_t::run_ExitWin() {
	endTime = std::chrono::system_clock::now();
	updateStats();
	setInProgressFlag(false);
	uitab[button_Guess]->setVisibleFlag(false);
	for (int i = kbbut_Q; i <= kbbut_M; i++)
		uitab[i]->setVisibleFlag(false);
	uitab[text_Won]->setVisibleFlag(true);

	while (1) {
		while (window->pollEvent(*event)) {
			if (event->type == sf::Event::MouseButtonReleased) {
				if (event->mouseButton.button == sf::Mouse::Button::Left) {
					if (uitab[button_Exit]->isActive()) {
						guessedLetters.clear();
						wrongLetters.clear();
						wrongTries = 0;
						uitab.hideAll();
						return Exit;
					}
				}
			}
		}
	}
}

game_t::State game_t::run_ExitLose() {
	endTime = std::chrono::system_clock::now();
	updateStats();
	setInProgressFlag(false);

	if (exitFlag == true) {
		guessedLetters.clear();
		wrongLetters.clear();
		wrongTries = 0;
		uitab.hideAll();
		return Exit;
	}

	wordReveal();
	uitab[button_Guess]->setVisibleFlag(false);
	for (int i = kbbut_Q; i <= kbbut_M; i++)
		uitab[i]->setVisibleFlag(false);
	uitab[text_Lost]->setVisibleFlag(true);

	while (1) {
		while (window->pollEvent(*event)) {
			if (event->type == sf::Event::MouseButtonReleased) {
				if (event->mouseButton.button == sf::Mouse::Button::Left) {
					if (uitab[button_Exit]->isActive()) {
						guessedLetters.clear();
						wrongLetters.clear();
						wrongTries = 0;
						uitab.hideAll();
						return Exit;
					}
				}
			}
		}
	}
}

game_t::State game_t::run_ExitError() {
	setInProgressFlag(false);
	guessedLetters.clear();
	wrongLetters.clear();
	wrongTries = 0;
	uitab.hideAll();
	return Exit;
}

game_t::game_t(sf::RenderWindow * window_, sf::Event * event_, vector<wordlist_t>* wordlistTab_, font_tab_t* fonts_, stats_t* stats_)
	:window{ window_ }, event{ event_ }, wordlistTab{ wordlistTab_ }, fonts{ fonts_ }, stats{ stats_ }{

	//text_GameText - displays the word currently in play
	uitab.registerElem(new text_t(50, 150, false, window, event, "", sf::Color::White, sf::Color::Black, 40, fonts->SegoeUI, 2));
	
	//text_Category - displays the category of the word in play
	uitab.registerElem(new text_t(50, 200, false, window, event, "", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2));

	//button_Exit - the exit button
	uitab.registerElem(new button_t("", 0, 0, false, window, event, "Quit", sf::Color::White, sf::Color::Black, 16, fonts->SegoeUI, 2, 60, 30, sf::Color::Red, sf::Color::Black));
	
	//button_Guess - the "guess the word" button
	uitab.registerElem(new button_t("", CENTER(WINDOW_W, 150), KB_VERTICAL_START - KB_SPACE_BETWEEN_KEYS - 30, false, window, event, "Guess the word", sf::Color::White, sf::Color::Black, 18, fonts->SegoeUI, 2, 150, 30, sf::Color::Green, sf::Color::Black));
	
	//input_Guess - the input field for guessing the word
	uitab.registerElem(new text_input_field_t(CENTER(WINDOW_W, 600), KB_VERTICAL_START - 2 * KB_SPACE_BETWEEN_KEYS - 30 - 40, false, window, event, "", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 600, 40));
	
	//text_ExitPrompt - displayed when the player wants to exit the game; "are you sure?" question
	uitab.registerElem(new text_t(CENTER(WINDOW_W, 420), 200, false, window, event, "Quit the game? (quitting means losing)", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2));
	
	//button_ExitYES
	uitab.registerElem(new button_t("", CENTER(WINDOW_W, 90) - 50, CENTER(WINDOW_H, 40), false, window, event, "YES", sf::Color::White, sf::Color::Black, 28, fonts->SegoeUI, 2, 90, 40, sf::Color::Green, sf::Color::Black));
	
	//button_ExitNO
	uitab.registerElem(new button_t("", CENTER(WINDOW_W, 90) + 50, CENTER(WINDOW_H, 40), false, window, event, "NO", sf::Color::White, sf::Color::Black, 28, fonts->SegoeUI, 2, 90, 40, sf::Color::Red, sf::Color::Black));
	
	//text_Lost
	uitab.registerElem(new text_t(CENTER(KB_WIDTH, 300), KB_VERTICAL_START-100 + CENTER(WINDOW_H - KB_VERTICAL_START, 60), false, window, event, "YOU'VE LOST", sf::Color::White, sf::Color::Black, 60, fonts->SegoeUI, 2));

	//text_Won
	uitab.registerElem(new text_t(CENTER(KB_WIDTH, 300), KB_VERTICAL_START-100 + CENTER(WINDOW_H - KB_VERTICAL_START, 60), false, window, event, "YOU'VE WON", sf::Color::White, sf::Color::Black, 60, fonts->SegoeUI, 2));

	//hangman's "bodyparts"
	//gallows base
	uitab.registerElem(new rectangle_t(HANGMAN_BASE_POS_X, HANGMAN_BASE_POS_Y, false, window, event, 100, 5, 0, sf::Color::White, sf::Color::White, 0));

	//gallows vertical beam
	uitab.registerElem(new rectangle_t(HANGMAN_BASE_POS_X, HANGMAN_BASE_POS_Y - 112, false, window, event, 5, 220, 0, sf::Color::White, sf::Color::White, 0));

	//gallows horizontal beam
	uitab.registerElem(new rectangle_t(HANGMAN_BASE_POS_X + 53, HANGMAN_BASE_POS_Y - 220, false, window, event, 100, 5, 0, sf::Color::White, sf::Color::White, 0));

	//rope lol
	uitab.registerElem(new rectangle_t(HANGMAN_BASE_POS_X + 100, HANGMAN_BASE_POS_Y - 205, false, window, event, 5, 30, 0, sf::Color::White, sf::Color::White, 0));

	//head
	uitab.registerElem(new circle_t(HANGMAN_BASE_POS_X + 80, HANGMAN_BASE_POS_Y - 190, false, window, event, 20, sf::Color::Transparent, sf::Color::White, -5));

	//body
	uitab.registerElem(new rectangle_t(HANGMAN_BASE_POS_X + 100, HANGMAN_BASE_POS_Y - 110, false, window, event, 5, 80, 0, sf::Color::White, sf::Color::White, 0));

	//left arm
	uitab.registerElem(new rectangle_t(HANGMAN_BASE_POS_X + 85, HANGMAN_BASE_POS_Y - 110, false, window, event, 5, 40, 45, sf::Color::White, sf::Color::White, 0));

	//right arm
	uitab.registerElem(new rectangle_t(HANGMAN_BASE_POS_X + 115, HANGMAN_BASE_POS_Y - 110, false, window, event, 5, 40, -45, sf::Color::White, sf::Color::White, 0));
	
	//left leg
	uitab.registerElem(new rectangle_t(HANGMAN_BASE_POS_X + 85, HANGMAN_BASE_POS_Y - 60, false, window, event, 5, 40, 45, sf::Color::White, sf::Color::White, 0));

	//right leg
	uitab.registerElem(new rectangle_t(HANGMAN_BASE_POS_X + 115, HANGMAN_BASE_POS_Y - 60, false, window, event, 5, 40, -45, sf::Color::White, sf::Color::White, 0));
	
	//virtual keyboard buttons
	uitab.registerElem(new button_t("q", KB1_LEFT_MARGIN, KB_VERTICAL_START, false, window, event, "Q", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("w", KB1_LEFT_MARGIN + KB_SPACE_BETWEEN_KEYS + KB_BUTTON_SIZE, KB_VERTICAL_START, false, window, event, "W", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("e", KB1_LEFT_MARGIN + 2 * KB_SPACE_BETWEEN_KEYS + 2 * KB_BUTTON_SIZE, KB_VERTICAL_START, false, window, event, "E", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("r", KB1_LEFT_MARGIN + 3 * KB_SPACE_BETWEEN_KEYS + 3 * KB_BUTTON_SIZE, KB_VERTICAL_START, false, window, event, "R", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("t", KB1_LEFT_MARGIN + 4 * KB_SPACE_BETWEEN_KEYS + 4 * KB_BUTTON_SIZE, KB_VERTICAL_START, false, window, event, "T", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("y", KB1_LEFT_MARGIN + 5 * KB_SPACE_BETWEEN_KEYS + 5 * KB_BUTTON_SIZE, KB_VERTICAL_START, false, window, event, "Y", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("u", KB1_LEFT_MARGIN + 6 * KB_SPACE_BETWEEN_KEYS + 6 * KB_BUTTON_SIZE, KB_VERTICAL_START, false, window, event, "U", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("i", KB1_LEFT_MARGIN + 7 * KB_SPACE_BETWEEN_KEYS + 7 * KB_BUTTON_SIZE, KB_VERTICAL_START, false, window, event, "I", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("o", KB1_LEFT_MARGIN + 8 * KB_SPACE_BETWEEN_KEYS + 8 * KB_BUTTON_SIZE, KB_VERTICAL_START, false, window, event, "O", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("p", KB1_LEFT_MARGIN + 9 * KB_SPACE_BETWEEN_KEYS + 9 * KB_BUTTON_SIZE, KB_VERTICAL_START, false, window, event, "P", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("a", KB2_LEFT_MARGIN, KB_VERTICAL_START + KB_BUTTON_SIZE + KB_SPACE_BETWEEN_KEYS, false, window, event, "A", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("s", KB2_LEFT_MARGIN + KB_SPACE_BETWEEN_KEYS + KB_BUTTON_SIZE, KB_VERTICAL_START + KB_BUTTON_SIZE + KB_SPACE_BETWEEN_KEYS, false, window, event, "S", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("d", KB2_LEFT_MARGIN + 2 * KB_SPACE_BETWEEN_KEYS + 2 * KB_BUTTON_SIZE, KB_VERTICAL_START + KB_BUTTON_SIZE + KB_SPACE_BETWEEN_KEYS, false, window, event, "D", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("f", KB2_LEFT_MARGIN + 3 * KB_SPACE_BETWEEN_KEYS + 3 * KB_BUTTON_SIZE, KB_VERTICAL_START + KB_BUTTON_SIZE + KB_SPACE_BETWEEN_KEYS, false, window, event, "F", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("g", KB2_LEFT_MARGIN + 4 * KB_SPACE_BETWEEN_KEYS + 4 * KB_BUTTON_SIZE, KB_VERTICAL_START + KB_BUTTON_SIZE + KB_SPACE_BETWEEN_KEYS, false, window, event, "G", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("h", KB2_LEFT_MARGIN + 5 * KB_SPACE_BETWEEN_KEYS + 5 * KB_BUTTON_SIZE, KB_VERTICAL_START + KB_BUTTON_SIZE + KB_SPACE_BETWEEN_KEYS, false, window, event, "H", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("j", KB2_LEFT_MARGIN + 6 * KB_SPACE_BETWEEN_KEYS + 6 * KB_BUTTON_SIZE, KB_VERTICAL_START + KB_BUTTON_SIZE + KB_SPACE_BETWEEN_KEYS, false, window, event, "J", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("k", KB2_LEFT_MARGIN + 7 * KB_SPACE_BETWEEN_KEYS + 7 * KB_BUTTON_SIZE, KB_VERTICAL_START + KB_BUTTON_SIZE + KB_SPACE_BETWEEN_KEYS, false, window, event, "K", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("l", KB2_LEFT_MARGIN + 8 * KB_SPACE_BETWEEN_KEYS + 8 * KB_BUTTON_SIZE, KB_VERTICAL_START + KB_BUTTON_SIZE + KB_SPACE_BETWEEN_KEYS, false, window, event, "L", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("z", KB3_LEFT_MARGIN, KB_VERTICAL_START + 2 * KB_BUTTON_SIZE + 2 * KB_SPACE_BETWEEN_KEYS, false, window, event, "Z", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("x", KB3_LEFT_MARGIN + KB_SPACE_BETWEEN_KEYS + KB_BUTTON_SIZE, KB_VERTICAL_START + 2 * KB_BUTTON_SIZE + 2 * KB_SPACE_BETWEEN_KEYS, false, window, event, "X", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("c", KB3_LEFT_MARGIN + 2 * KB_SPACE_BETWEEN_KEYS + 2 * KB_BUTTON_SIZE, KB_VERTICAL_START + 2 * KB_BUTTON_SIZE + 2 * KB_SPACE_BETWEEN_KEYS, false, window, event, "C", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("v", KB3_LEFT_MARGIN + 3 * KB_SPACE_BETWEEN_KEYS + 3 * KB_BUTTON_SIZE, KB_VERTICAL_START + 2 * KB_BUTTON_SIZE + 2 * KB_SPACE_BETWEEN_KEYS, false, window, event, "V", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("b", KB3_LEFT_MARGIN + 4 * KB_SPACE_BETWEEN_KEYS + 4 * KB_BUTTON_SIZE, KB_VERTICAL_START + 2 * KB_BUTTON_SIZE + 2 * KB_SPACE_BETWEEN_KEYS, false, window, event, "B", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("n", KB3_LEFT_MARGIN + 5 * KB_SPACE_BETWEEN_KEYS + 5 * KB_BUTTON_SIZE, KB_VERTICAL_START + 2 * KB_BUTTON_SIZE + 2 * KB_SPACE_BETWEEN_KEYS, false, window, event, "N", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));
	uitab.registerElem(new button_t("m", KB3_LEFT_MARGIN + 6 * KB_SPACE_BETWEEN_KEYS + 6 * KB_BUTTON_SIZE, KB_VERTICAL_START + 2 * KB_BUTTON_SIZE + 2 * KB_SPACE_BETWEEN_KEYS, false, window, event, "M", sf::Color::White, sf::Color::Black, 24, fonts->SegoeUI, 2, 60, 60, sf::Color(128, 128, 128, 255), sf::Color::Black));

	currentState = Start;
	fnctab[Start] = &game_t::run_Start;
	fnctab[Main] = &game_t::run_Main;
	fnctab[ExitPrompt] = &game_t::run_ExitPrompt;
	fnctab[ExitWin] = &game_t::run_ExitWin;
	fnctab[ExitLose] = &game_t::run_ExitLose;
	fnctab[ExitError] = &game_t::run_ExitError;
}

void game_t::run() {
	while (currentState != Exit) {
		currentState = runState(currentState);
	}
	currentState = Start;
}

void game_t::drawUI() {
	for (int i = 0; i < UIID_COUNT; i++) {
		if (uitab[i]->isVisible()) {
			uitab[i]->draw();
		}
	}
}

void game_t::pauseSave() {
	for (int i = 0; i < UIID_COUNT; i++)
		pauseSavedVisibleFlags.push_back(uitab[i]->isVisible());
}

void game_t::pauseRecovery() {
	for (int i = 0; i < UIID_COUNT; i++)
		uitab[i]->setVisibleFlag(pauseSavedVisibleFlags[i]);
}

bool game_t::isGuessed(char letter) {
	for (int i = 0; i < guessedLetters.size(); i++) {
		if (guessedLetters[i] == letter) {
			return true;
		}
	}
	return false;
}

void game_t::createGameText() {
	std::string gameString;
	for (int i = 0; i < gameWord.getWord().size(); i++) {
		if (gameWord.getWord()[i] == ' ')
			gameString += ' ';
		else
			gameString += '_';

		gameString += ' ';
	}
	uitab[text_GameText]->setString(gameString);
}

void game_t::updateGameText(char letter) {
	std::string tmpGameString = uitab[text_GameText]->getString();
	for (int i = 0; i < gameWord.getWord().size(); i++) {
		if (gameWord.getWord()[i] == letter) {
			tmpGameString[i * 2] = letter;
		}
	}
	uitab[text_GameText]->setString(tmpGameString);
}

void game_t::wordReveal() {
	for (int i = 0; i < gameWord.getUniqueLetterList().size(); i++) {
		updateGameText(gameWord.getUniqueLetterList()[i]);
	}
}

void game_t::updateStats() {
	if (currentState == ExitWin)
		stats->addVictory();
	else if (currentState == ExitLose)
		stats->addDefeat();

	std::chrono::duration<double> gameTime = endTime - startTime;
	stats->addGameTime(gameTime.count());
}
