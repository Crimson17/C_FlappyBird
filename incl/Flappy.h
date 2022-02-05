#pragma once

#include "Console.h"
#include "Size.h"
#include "Player.h"


class Flappy {
public:
	// Constructor
	Flappy();

private:
	// Frame size
	Size frameSize;

	// Player info
	Player player;

	// States
	void login();
	void menu();
	void game();
	void search();
};
