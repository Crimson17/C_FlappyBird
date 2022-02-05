#pragma once

#include <string>

#include "Frame.h"
#include "Size.h"


class Player {
public:
	std::string name;
	float position;
	float velocity;

	int spaces;
	int pillars;

	Player();
	Player(const std::string&);

	void phys(float, float);
	bool draw(const Frame&, const Size&);
};
