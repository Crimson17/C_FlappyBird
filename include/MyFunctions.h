#include "..\include\MyStructures.h"

#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H

void Input();

char **AllocateFrameMemory(int frameWidth, int frameHeight);
void FreeFrameMemory(char **frame, int frameHeight);
void ClearFrame(char **frame, int frameWidth, int frameHeight);
void DisplayFrame(char **frame, int frameHeight);

int PointInFrame(int frameHeight, int frameWidth, int x, int y);

void UpdatePhysics(float *playerPosition, float *playerVelocity, float gravity, int fps);
void SetPlayer(char **frame, int frameWidth, int frameHeight, int position);
void PillarLogic(PILLAR *pillars, int n, int frameCount, int frameHeight, int frameWidth);
void SetPillars(char **frame, int frameWidth, int frameHeight, PILLAR *pillars, int n);

void SaveUserScore(const char *fileName, SCORE *data);

#endif