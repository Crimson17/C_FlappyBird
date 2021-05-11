#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "..\include\MyFunctions.h"
#include "..\include\MyStructures.h"

char **AllocateFrameMemory(int frameWidth, int frameHeight)
{
    char **matrix = (char **)calloc(frameHeight, sizeof(char *));
    if (matrix == NULL)
    {
        exit(-1);
    }
    for (int i = 0; i < frameHeight; i++)
    {
        *(matrix + i) = (char *)calloc(frameWidth + 2, sizeof(char));
        if (*(matrix + i) == NULL)
        {
            free(matrix);
            exit(-1);
        }
    }
    return matrix;
}

void FreeFrameMemory(char **frame, int frameHeight)
{
    for (int i = 0; i < frameHeight; i++)
    {
        free(*(frame + i));
    }
    free(frame);
}

void ClearFrame(char **frame, int frameWidth, int frameHeight)
{
    int y, x;
    for (y = 0; y < frameHeight; y++)
    {
        for (x = 0; x < frameWidth; x++)
        {
            *(*(frame + y) + x) = ' ';
        }
        *(*(frame + y) + x) = '\n';
        *(*(frame + y) + x + 1) = '\0';
    }
}

void SetPlayerPos(char **frame, int frameWidth, int frameHeight, int playerPosition)
{
    if (playerPosition >= 0 && playerPosition < frameHeight)
    {
        if (*(*(frame + (int)playerPosition) + frameWidth / 4) != '#')
        {
            *(*(frame + (int)playerPosition) + frameWidth / 4) = '@';
        }
        else
        {
            globalRunning = 0;
        }
    }
    else
    {
        globalRunning = 0;
    }
}

void UpdatePhysics(float *playerPosition, float *playerVelocity, float gravity, int fps)
{
    *playerVelocity += (gravity / fps);
    *playerPosition += (*playerVelocity / fps);
}

void SpawnPillair(char **frame, int frameHeight, int xPos, int yPos)
{
    // Upper pillair
    for (int y = 0; y < (yPos - 2); y++)
    {
        int xLen = y < (yPos - 4) ? xPos + 3 : xPos + 4;
        int xStart = y < (yPos - 4) ? xPos - 1 : xPos - 2;
        for (int x = xStart; x < xLen; x++)
        {
            *(*(frame + y) + x) = '#';
        }
    }
    // Lower pillair
    for (int y = yPos + 2; y < frameHeight; y++)
    {
        int xLen = y > (yPos + 3) ? xPos + 3 : xPos + 4;
        int xStart = y > (yPos + 3) ? xPos - 1 : xPos - 2;
        for (int x = xStart; x < xLen; x++)
        {
            *(*(frame + y) + x) = '#';
        }
    }
}

void DisplayFrame(char **frame, int frameHeight)
{
    for (int i = 0; i < frameHeight; i++)
    {
        printf("%s", *(frame + i));
    }
}