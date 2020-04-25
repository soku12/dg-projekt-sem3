#pragma once
#include "vector.h"
#include "word.h"
#include <string>
#include <fstream>

class wordlist_t {
	vector<word_t> wtab;
	bool loadedFlag;
	bool errorFlag;
	std::string filename;
public:
	wordlist_t() = default;
	wordlist_t(std::string filename_);
	wordlist_t(const wordlist_t& param) :wtab{ param.wtab } {}
	~wordlist_t() = default;
	void operator=(const wordlist_t& param);
	void setLoadedFlag(bool val) { loadedFlag = val; }
	bool isLoaded() { return loadedFlag; }
	int getWordCount() { return wtab.size(); }
	bool getErrorFlag() { return errorFlag; }
	bool isValid();
	std::string getFilename() { return filename; }
	word_t& operator[](const int iter) { return wtab[iter]; }
};

