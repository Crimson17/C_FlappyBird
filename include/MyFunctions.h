#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H

char **AllocateFrameMemory(int frameWidth, int frameHeight);
void FreeFrameMemory(char **matrix, int frameHeight);
void ClearFrame(char **frame, int frameWidth, int frameHeight);
void DisplayFrame(char **frame, int frameHeight);

void SetPlayerPos(char **frame, int frameWidth, int frameHeight, int position);
void UpdatePhysics(float *playerPosition, float *playerVelocity, float gravity, int fps);
void SpawnPillair(char **frame, int frameHeight, int xPos, int yPos);

#endif