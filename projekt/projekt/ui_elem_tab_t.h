#pragma once
#include <vector>
#include "vector.h"
#include "ui_elem.h"

class ui_elem_tab_t {
	vector<ui_elem_t*> tab;
public:
	ui_elem_tab_t() = default;
	ui_elem_tab_t(const ui_elem_tab_t& copy) { tab = copy.tab; }
	~ui_elem_tab_t();
	void registerElem(ui_elem_t* elem_) { tab.push_back(elem_); }
	void hideAll();
	ui_elem_t*& operator[](int iter_) { return tab[iter_]; }
};

