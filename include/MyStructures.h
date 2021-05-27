#ifndef MYSTRUCTURES_H
#define MYSTRUCTURES_H

// Player properties
typedef struct Player
{
	float position;
	float velocity;
} PLAYER;

// Players score
typedef struct Score
{
	char playerName[18];
	int spaceCounter;
	int pillarsPassed;
	float timeSurvived;
} SCORE;

// Pillar properties
typedef struct Pillar
{
	int x;
	int y;
} PILLAR;

// Global variables
int _globalRunning;
char _playerName[18];

// Console setup
HANDLE hStdOut;
CONSOLE_CURSOR_INFO curInfo;

#endif