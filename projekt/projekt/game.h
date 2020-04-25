#pragma once
#include "ui_elem_tab_t.h"
#include "ui_elem.h"
#include "wordlist.h"
#include "text_t.h"
#include "button.h"
#include "macros.h"
#include "stats.h"
#include "font_tab_t.h"
#include "text_input_field_t.h"
#include "rectangle_t.h"
#include "circle_t.h"
#include <random>
#include <chrono>



class game_t {
	enum State{Start, Main, ExitPrompt, ExitWin, ExitLose, ExitError, Exit, STATE_COUNT};
	enum UIID{text_GameText, text_Category, button_Exit, button_Guess, input_Guess, text_ExitPrompt, button_ExitYES, button_ExitNO, text_Lost, text_Won,
		rect_HMBase, rect_HMVertBeam, rect_HMHorizBeam, rect_HMRope, circ_HMHead, rect_HMBody,
		rect_HMLeftArm, rect_HMRightArm, rect_HMLeftLeg, rect_HMRightLeg,
		kbbut_Q, kbbut_W, kbbut_E, kbbut_R, kbbut_T, kbbut_Y, kbbut_U, kbbut_I, kbbut_O, kbbut_P, 
		kbbut_A, kbbut_S, kbbut_D, kbbut_F, kbbut_G, kbbut_H, kbbut_J, kbbut_K, kbbut_L,
		kbbut_Z, kbbut_X, kbbut_C, kbbut_V, kbbut_B, kbbut_N, kbbut_M, UIID_COUNT};
	
	ui_elem_tab_t uitab;
	sf::Event* event;
	sf::RenderWindow* window;
	vector<wordlist_t>* wordlistTab;
	font_tab_t* fonts;
	stats_t* stats;

	word_t gameWord;
	vector<char> guessedLetters;
	vector<char> wrongLetters;
	int wrongTries;

	std::chrono::time_point<std::chrono::system_clock> startTime;
	std::chrono::time_point<std::chrono::system_clock> endTime;

	vector<bool> pauseSavedVisibleFlags;
	bool inProgressFlag;
	bool exitFlag;

	State currentState;
	State(game_t::*fnctab[STATE_COUNT])();

	State runState(State wantedState);
	State run_Start();
	State run_Main();
	State run_ExitPrompt();
	State run_ExitWin();
	State run_ExitLose();
	State run_ExitError();

	bool getInProgressFlag() { return inProgressFlag; }
	void setInProgressFlag(bool val) { inProgressFlag = val; }
	void pauseSave();
	void pauseRecovery();
	bool isGuessed(char letter);
	void createGameText();
	void updateGameText(char letter);
	void wordReveal();
	void updateStats();

public:
	game_t(sf::RenderWindow* window_, sf::Event* event_, vector<wordlist_t>* wordlistTab_, font_tab_t* fonts_, stats_t* stats_);
	~game_t() = default;
	void run();
	void drawUI();
};

