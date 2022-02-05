#pragma once

#include <string>
#include <conio.h>
#include <windows.h>

#include "Size.h"


// Returns the console handle
class Console {
public:
	// Constructor
	Console();

	// Cursor
	void hideC();
	void moveC(int, int);

	// Frame
	void clear();
	Size size();

	// Input
	char input();

	// Title
	void title(const std::string&);

private:
	HANDLE handle = nullptr;
};
