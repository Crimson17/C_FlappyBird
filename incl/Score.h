#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>

#include "Size.h"


class Score {
public:
	char name[20];
	int spaces;
	int pillars;
	float time;

	Score();
	Score(const std::string&);

	// File
	void save(const std::string&);
	static void list(const std::string&, const Size&);
	static void list(const std::string&, const std::string&, const Size&);

private:
	// File
	static std::vector<Score> read(const std::string&);
};
