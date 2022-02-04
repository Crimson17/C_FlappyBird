#pragma once

#include <windows.h>


class Timer {
public:
	Timer();

	// Returns the interval between 2 calls
	float interval();

	// Resets the last time
	void reset();

	// Returns the passed time since the last reset
	float elapsed();

private:
	float recFreq = 0;
	LARGE_INTEGER inStartTime = {};
	LARGE_INTEGER swStartTime = {};

	// Stores the pc frequency
	void storeFreq();

	// Calculates elapsed time from 2 given LARGE_INTEGERS
	float calcElapTime(const LARGE_INTEGER&, const LARGE_INTEGER&);
};


