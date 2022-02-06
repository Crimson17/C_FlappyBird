#pragma once

#include <string>
#include <conio.h>
#include <windows.h>

#include "Size.h"


// Returns the console handle
class Console {
public:
	// Constructor deletion
	Console() = delete;

	// Cursor
	static void hideC();
	static void moveC(int, int);

	// Frame
	static void clear();
	static Size size();

	// Input
	static char input();

	// Title
	static void title(const std::string&);

private:
	static HANDLE handle;
};
