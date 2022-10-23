#pragma once

#include <windows.h>


class timer
{
	float rec_frequency_ = 1.0f;
	LARGE_INTEGER interval_start_time_ = {};
	LARGE_INTEGER stopwatch_start_time_ = {};

	float calculate_time(const LARGE_INTEGER&, const LARGE_INTEGER&) const;
	
public:
	timer();

	void reset();
	
	[[nodiscard]] float get_elapsed() const;
	[[nodiscard]] float get_interval();
};


