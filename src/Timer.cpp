#include "Timer.h"


Timer::Timer() {
	this->storeFreq();
	this->interval();
	this->reset();
}

// Returns the interval between 2 calls
float Timer::interval() {
	LARGE_INTEGER inEndTime = {};
	QueryPerformanceCounter(&inEndTime);
	const float elapsedTime = calcElapTime(inStartTime, inEndTime);
	this->inStartTime = inEndTime;
	return elapsedTime;
}

// Resets the last time
void Timer::reset() {
	QueryPerformanceCounter(&this->swStartTime);
}

// Returns the passed time since the last reset
float Timer::elapsed() {
	LARGE_INTEGER swEndTime = {};
	QueryPerformanceCounter(&swEndTime);
	return calcElapTime(this->swStartTime, swEndTime);
}

// Stores the pc frequency
void Timer::storeFreq() {
	LARGE_INTEGER tempFreq = {};
	QueryPerformanceFrequency(&tempFreq);
	this->recFreq = 1.0f / tempFreq.QuadPart;
}

// Calculates elapsed time from 2 given LARGE_INTEGERS
float Timer::calcElapTime(const LARGE_INTEGER& startTime, const LARGE_INTEGER& endTime) {
	return (endTime.QuadPart - startTime.QuadPart) * this->recFreq;
}
