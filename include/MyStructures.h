#ifndef MYSTRUCTURES_H
#define MYSTRUCTURES_H

typedef struct Score
{
    int spaceCounter;
    int pillarsPassed;
    float timeSurvived;
} SCORE;

typedef struct Pillar{
    int x;
    int y;
}PILLAR;

int globalRunning;

#endif