#ifndef MYSTRUCTURES_H
#define MYSTRUCTURES_H

// Player...
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

// Pillar...
typedef struct Pillar
{
    int x;
    int y;
} PILLAR;

// Game loop condition
int globalRunning;

#endif