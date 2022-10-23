#include "console.h"

#include <conio.h>


HANDLE console::handle_ = GetStdHandle(STD_OUTPUT_HANDLE);

void console::hide_cursor()
{
	CONSOLE_CURSOR_INFO info = {};
	GetConsoleCursorInfo(handle_, &info);
	info.bVisible = FALSE;
	SetConsoleCursorInfo(handle_, &info);
}

void console::move_cursor(const int x, const int y)
{
	const COORD position = { static_cast<short>(x), static_cast<short>(y) };
	SetConsoleCursorPosition(console::handle_, position);
}

void console::clear()
{
	system("cls");
}

size console::size()
{
	CONSOLE_SCREEN_BUFFER_INFO info = {};
	GetConsoleScreenBufferInfo(handle_, &info);
	return {info.srWindow.Right - info.srWindow.Left + 1, info.srWindow.Bottom - info.srWindow.Top + 1};
}

char console::read_input()
{
	char result = 0;
	while (_kbhit()) {
		result = _getch();
	}
	return result;
}

void console::title(const std::string& title)
{
	SetConsoleTitleA(title.c_str());
}
