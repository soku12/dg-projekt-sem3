#include "wordlist.h"

wordlist_t::wordlist_t(std::string filename_) :filename{ filename_ } {
	std::ifstream file(filename);
	
	if (!file) {
		loadedFlag = false;
		errorFlag = true;
	}
	else {
		std::string line;
		int lineNumber = 0;
		std::string word;
		while (std::getline(file, line)) {
			lineNumber++;
			if (lineNumber % 2 == 1) { //if lineNumber is odd
				word = line;
			}
			else { //if linenumber is even
				wtab.push_back({ word, line });
			}
		}
		loadedFlag = true;
		errorFlag = false;
	}
}

void wordlist_t::operator=(const wordlist_t & param) {
	wtab = param.wtab;
	loadedFlag = param.loadedFlag;
	errorFlag = param.errorFlag;
	filename = param.filename;
}

bool wordlist_t::isValid() {
	if (errorFlag == false) {
		if (loadedFlag == true) {
			if (getWordCount() > 0) {
				return true;
			}
		}
	}
	return false;
}
