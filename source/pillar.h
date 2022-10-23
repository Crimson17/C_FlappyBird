#pragma once

#include "Size.h"
#include "Frame.h"


class pillar
{
public:
	float x = 0.0f;
	float y = 0.0f;

	float speed = 20.0f;

	bool can_pass = true;

	pillar();
	pillar(float x, float y);

	int phys(const size& frame_size, float delta_t);
	void draw(const frame& frame, const size& frame_size) const;
};

