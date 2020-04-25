#pragma once

//std::vector for poor people
//requires:
//	operator=
//	default constructor
//	copy constructor

template <typename T>
class vector { 
	T* tab;
	int siz;

public:
	vector() :tab{ nullptr }, siz{ 0 }{}


	vector(const vector<T>& param) {
		tab = new T[param.siz];
		siz = param.siz;

		for (int i = 0; i < param.siz; i++) {
			tab[i] = param.tab[i];
		}
	}


	~vector() {
		if (siz > 0)
			delete[] tab;
	}


	void push_back(T elem) { //add a new element at the end of the table
		T* newtab = new T[siz + 1];

		for (int i = 0; i < siz; i++) {
			newtab[i] = tab[i];
		}
		newtab[siz] = elem;

		if (siz > 0)
			delete[] tab;

		tab = newtab;
		siz += 1;
	}


	inline int size() { return siz; }


	T& operator[](int iter) {
		return tab[iter];
	}

	void operator=(const vector<T>& param) {
		if (tab != nullptr) {
			delete[] tab;
		}
		tab = new T[param.siz];
		siz = param.siz;

		for (int i = 0; i < param.siz; i++) {
			tab[i] = param.tab[i];
		}
	}

	bool operator==(const vector<T>& param) {
		if (siz != param.siz)
			return false;
		else {
			for (int i = 0; i < siz; i++) {
				if (tab[i] != param.tab[i])
					return false;
			}
			return true;
		}
	}

	void clear() {
		if (siz > 0) {
			delete[] tab;
			siz = 0;
		}
	}
};

