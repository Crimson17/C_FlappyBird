#pragma once

#include "size.h"
#include "player.h"


class flappy
{
	size frame_size_ = {};
	player player_ = {};
	
	void login();
	void menu();
	void game();
	void search() const;
	
public:
	flappy();
};
