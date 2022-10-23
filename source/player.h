#pragma once

#include <string>

#include "frame.h"
#include "size.h"


class player
{
public:
	std::string name = "_unknown";
	float position = 0.0f;
	float velocity = 0.0f;

	player();
	explicit player(const std::string& name);

	void phys_update(float gravity, float delta_t);
	[[nodiscard]] bool draw(const frame& frame, const size& frame_size) const;
};
