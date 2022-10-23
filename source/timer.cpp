#include "timer.h"


timer::timer()
{
	LARGE_INTEGER frequency = {};
	QueryPerformanceFrequency(&frequency);
	rec_frequency_ = 1.0f / static_cast<float>(frequency.QuadPart);
	reset();
}

void timer::reset()
{
	QueryPerformanceCounter(&stopwatch_start_time_);
	QueryPerformanceCounter(&interval_start_time_);
}

float timer::get_interval()
{
	LARGE_INTEGER current_time = {};
	QueryPerformanceCounter(&current_time);
	const float result = calculate_time(interval_start_time_, current_time);
	interval_start_time_ = current_time;
	return result;
}

float timer::get_elapsed() const
{
	LARGE_INTEGER current_time = {};
	QueryPerformanceCounter(&current_time);
	return calculate_time(stopwatch_start_time_, current_time);
}

float timer::calculate_time(const LARGE_INTEGER& start, const LARGE_INTEGER& end) const
{
	return static_cast<float>(end.QuadPart - start.QuadPart) * rec_frequency_;
}
