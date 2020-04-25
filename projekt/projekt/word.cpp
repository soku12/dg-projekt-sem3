#include "word.h"

word_t::word_t(std::string word_, std::string category_) :word{ word_ }, category{ category_ } {
	for (int i = 0; i < word.size(); i++) {
		if (checkLetter(word[i]) == false) {
			if (word[i] != ' ')
				uniqueLetterList.push_back(word[i]);
		}
	}
}

void word_t::operator=(const word_t& param) {
	word = param.word;
	category = param.category;
	uniqueLetterList = param.uniqueLetterList;
}

bool word_t::checkLetter(char & letter) {
	for (int i = 0; i < uniqueLetterList.size(); i++) {
		if (letter == uniqueLetterList[i]) {
			return true;
		}
	}
	return false;
}
