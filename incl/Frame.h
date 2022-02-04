#pragma once

#include <string>

#include "Size.h"


class Frame {
public:
	std::string data;

	Frame(Size);

	void clear();
	void display();
};

