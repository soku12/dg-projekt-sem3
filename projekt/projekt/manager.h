#pragma once
#include "ui_elem_tab_t.h"
#include "button.h"
#include "text_input_field_t.h"
#include "wordlist.h"
#include "font_tab_t.h"

class manager_t {
	enum State{Main, Load, Unload, Exit, STATE_COUNT};
	enum UIID{text_Title, text_LoadedWordlists, button_Exit, button_Load, button_Unload, input_InputField, text_Instruction, UIID_COUNT};

	ui_elem_tab_t uitab;
	sf::RenderWindow* window;
	sf::Event* event;
	vector<wordlist_t>* wordlistTab;
	font_tab_t* fonts;

	State currentState;
	State(manager_t::*fnctab[STATE_COUNT])();

	State runState(State wantedState);
	State run_Main();
	State run_Load();
	State run_Unload();

	void updateLoadedWordlistsString();

public:
	manager_t() = default;
	manager_t(sf::RenderWindow* window_, sf::Event* event_, vector<wordlist_t>* wordlistTab_, font_tab_t* fonts_);
	manager_t(const manager_t& copy);
	~manager_t() = default;
	void run();
	void drawUI();
};

