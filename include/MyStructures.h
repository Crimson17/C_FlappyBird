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

// Game loop condition
int _globalRunning;
char _playerName[18];

#endif