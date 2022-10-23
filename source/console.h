#pragma once

#include <string>
#include <windows.h>

#include "size.h"


class console
{
	static HANDLE handle_;
	
public:
	console() = delete;

	static void hide_cursor();
	static void move_cursor(int x, int y);

	static void clear();
	static size size();

	static char read_input();
	
	static void title(const std::string& title);
};
