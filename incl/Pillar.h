#pragma once

#include "Size.h"
#include "Frame.h"


class Pillar {
public:
	float x;
	float y;

	float speed;

	bool canPass;

	Pillar();
	Pillar(float, float);

	int phys(const Size&, float);
	void draw(const Frame&, const Size&);
};

