#include "ui_elem_tab_t.h"

ui_elem_tab_t::~ui_elem_tab_t() {
	for (int i = 0; i < tab.size(); i++) {
		delete tab[i];
	}
}

void ui_elem_tab_t::hideAll() {
	for (int i = 0; i < tab.size(); i++) {
		tab[i]->setVisibleFlag(false);
	}
}




