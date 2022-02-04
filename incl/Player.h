#pragma once

#include <string>

#include "Frame.h"
#include "Size.h"


class Player {
public:
	std::string name;
	float position;
	float velocity;

	int spaceCounter;
	int pillarsPassed;

	Player();
	Player(const std::string&);

	void phys(float, float);
	bool draw(const Frame&, const Size&);
};
