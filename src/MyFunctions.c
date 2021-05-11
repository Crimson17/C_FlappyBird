#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include "..\include\MyFunctions.h"
#include "..\include\MyStructures.h"

void Input(float *playerVelocity, SCORE *playerScore)
{
    if (kbhit())
    {
        char c;
        while (kbhit())
        {
            c = _getch();
        }
        if (c == 32)
        {
            *playerVelocity -= 10;
            playerScore->spaceCounter++;
        }
        else if (c == 27)
        {
            globalRunning = 0;
        }
    }
}

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

void UpdatePhysics(float *playerPosition, float *playerVelocity, float gravity, int fps)
{
    *playerVelocity += (gravity / fps);
    *playerPosition += (*playerVelocity / fps);
}

void SetPlayer(char **frame, int frameWidth, int frameHeight, int playerPosition)
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

int PointInFrame(int frameWidth, int frameHeight, int x, int y)
{
    if (x < 0 || x >= frameWidth || y < 0 || y >= frameHeight)
    {
        return 0;
    }
    return 1;
}

void PillarLogic(PILLAR *pillars, int n, int frameCount, int frameHeight, int frameWidth)
{
    for (int i = 0; i < n; i++)
    {
        (pillars + i)->x--;
        if ((pillars + i)->x == -4)
        { // -4 ostavi vise prostora izmedu svakih (_frameWidth / 20) + 2 pillera da budem barem malo lakse za igrati :)
            (pillars + i)->x = frameWidth + 22;
            (pillars + i)->y = 5 + (float)rand() / RAND_MAX * (frameHeight - 10);
        }
    }
}

void SetPillars(char **frame, int frameWidth, int frameHeight, PILLAR *pillars, int n)
{
    for (int i = 0; i < n; i++)
    {
        // Upper pillair
        for (int y = 0; y < ((pillars + i)->y - 2); y++)
        {

            int xLen = y < ((pillars + i)->y - 4) ? (pillars + i)->x + 3 : (pillars + i)->x + 4;
            int xStart = y < ((pillars + i)->y - 4) ? (pillars + i)->x - 1 : (pillars + i)->x - 2;
            for (int x = xStart; x < xLen; x++)
            {
                if (PointInFrame(frameWidth, frameHeight, x, y))
                {
                    *(*(frame + y) + x) = '#';
                }
            }
        }
        // Lower pillair
        for (int y = (pillars + i)->y + 2; y < frameHeight; y++)
        {
            int xLen = y > ((pillars + i)->y + 3) ? (pillars + i)->x + 3 : (pillars + i)->x + 4;
            int xStart = y > ((pillars + i)->y + 3) ? (pillars + i)->x - 1 : (pillars + i)->x - 2;
            for (int x = xStart; x < xLen; x++)
            {
                if (PointInFrame(frameWidth, frameHeight, x, y))
                {
                    *(*(frame + y) + x) = '#';
                }
            }
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

void SaveUserScore(const char *fileName, SCORE *data)
{
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        fp = fopen(fileName, "w");
        if (fp == NULL)
        {
            exit(-1);
        }
        fclose(fp);
    }
    else
    {
        fclose(fp);
    }
    fp = fopen(fileName, "a");
    if (fp == NULL)
    {
        exit(-1);
    }
    fprintf(fp, "Pillars passed: %d, Spacebar presses: %d, Time taken: %.2f seconds\n", data->pillarsPassed, data->spaceCounter, data->timeSurvived);
    fclose(fp);
}