#pragma once

#include "size.h"

#include <string>


class frame
{
public:
	std::string data = {};

	explicit frame(const size& size);

	void clear();
	void display() const;
};

