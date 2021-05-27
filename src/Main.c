//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include "../include/MyFunctions.h"
#include "../include/MyStructures.h"

// Game properties
const int _frameWidth = 119; // Default console width for 16 font size (shouldn't be less than this default value)
const int _frameHeight = 29; // Default console height for 16 font size (shouldn't be less than this default value)
const int _fps = 15;		 // Game fps (15 actually works really good)
const float _gravity = 30;	 // Player gravity (10 is too slow)

int main(void)
{
	// For random pillar generation
	srand((unsigned)time(NULL));

	// Console setup
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleCursorInfo(hStdOut, &curInfo);
	curInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hStdOut, &curInfo);

	// Calling the game functions
	NewPlayer(_frameWidth, _frameHeight);
	Menu(_frameWidth, _frameHeight, _fps, _gravity);

	return 0;
}