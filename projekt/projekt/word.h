#pragma once
#include <string>
#include "vector.h"

class word_t {
	std::string word;
	std::string category;
	vector<char> uniqueLetterList;

public:
	word_t() = default;
	word_t(std::string word_, std::string category_);
	word_t(const word_t& param) :word{ param.word } {}
	~word_t() = default;
	void operator=(const word_t& param);
	bool checkLetter(char& letter);
	std::string getWord() { return word; }
	std::string getCategory() { return category; }
	vector<char> getUniqueLetterList() { return uniqueLetterList; }
};

