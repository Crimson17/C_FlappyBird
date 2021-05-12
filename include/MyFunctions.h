#include "..\include\MyStructures.h"

#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H

// Memory functions
PILLAR *AllocatePillarMemory(int pillarCount);
char **AllocateFrameMemory(int frameWidth, int frameHeight, PILLAR *pillars);
void FreeFrameMemory(char **frame, int frameHeight);

// Default pillar configuration
void PillarConstructor(PILLAR *pillars, int pillarCount, int frameWidth, int frameHeight);

// Input function
void Input(PLAYER *player, SCORE *score);

// Frame functions
void ClearFrame(char **frame, int frameWidth, int frameHeight);
void DisplayFrame(char **frame, int frameHeight);

// Player and pillar functions
int PointInFrame(int frameHeight, int frameWidth, int x, int y);
void PillarLogic(PILLAR *pillars, int pillarCount, int frameHeight, int frameWidth, SCORE *score);
void SetPillars(char **frame, int frameWidth, int frameHeight, PILLAR *pillars, int pillarCount);
void UpdatePlayerPhysics(PLAYER *player, float gravity, int fps);
void SetPlayer(char **frame, int frameWidth, int frameHeight, PLAYER *player);

// Score function
void SaveUserScore(const char *fileName, SCORE *data);

#endif