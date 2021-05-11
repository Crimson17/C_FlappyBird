#include "..\include\MyStructures.h"

#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H

char **AllocateFrameMemory(int frameWidth, int frameHeight);
void FreeFrameMemory(char **frame, int frameHeight);
void ClearFrame(char **frame, int frameWidth, int frameHeight);
void DisplayFrame(char **frame, int frameHeight);

int PointInFrame(int frameHeight, int frameWidth, int x, int y);

void SetPlayer(char **frame, int frameWidth, int frameHeight, int position);
void UpdatePhysics(float *playerPosition, float *playerVelocity, float gravity, int fps);
void SetPillars(char **frame, int frameWidth, int frameHeight, PILLAR *pillars, int n);

void SaveUserScore(const char *fileName, SCORE *data);

#endif