#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include "..\include\MyFunctions.h"
#include "..\include\MyStructures.h"

// Allocate memory for the pillars
PILLAR *AllocatePillarMemory(int pillarCount)
{
    PILLAR *pillars = (PILLAR *)calloc(pillarCount, sizeof(PILLAR));
    if (pillars == NULL)
    {
        exit(-1);
    }
    return pillars;
}

// Allocates memory for a frame(char matrix)
char **AllocateFrameMemory(int frameWidth, int frameHeight, PILLAR *pillars)
{
    char **matrix = (char **)calloc(frameHeight, sizeof(char *));
    if (matrix == NULL)
    {
        free(pillars);
        exit(-1);
    }
    for (int i = 0; i < frameHeight; i++)
    {
        *(matrix + i) = (char *)calloc(frameWidth + 2, sizeof(char)); // + 2 for the '\n' and '\0'
        if (*(matrix + i) == NULL)
        {
            free(pillars);
            free(matrix);
            exit(-1);
        }
    }
    return matrix;
}

// Frees matrix memory
void FreeFrameMemory(char **frame, int frameHeight)
{
    for (int i = 0; i < frameHeight; i++)
    {
        free(*(frame + i));
    }
    free(frame);
}

// Default pillar configuration
void PillarConstructor(PILLAR *pillars, int pillarCount, int frameWidth, int frameHeight)
{
    for (int i = 0; i < pillarCount; i++)
    {
        (pillars + i)->x = frameWidth + 2 + (i * 20);
        (pillars + i)->y = 5 + (float)rand() / RAND_MAX * (frameHeight - 10);
    }
}

// Handles the keyboard input (SPACEBAR and ESC)
void Input(PLAYER *player, SCORE *score)
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
            player->velocity -= 10;
            score->spaceCounter++;
        }
        else if (c == 27)
        {
            _globalRunning = 0;
        }
    }
}

// Resets all frame chars to space
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

// Prints all chars to the terminal
void DisplayFrame(char **frame, int frameHeight)
{
    for (int i = 0; i < frameHeight; i++)
    {
        printf("%s", *(frame + i));
    }
}

// Returns true if the "point" is visible (in frame) or false if it's not
int PointInFrame(int frameWidth, int frameHeight, int x, int y)
{
    if (x < 0 || x >= frameWidth || y < 0 || y >= frameHeight)
    {
        return 0;
    }
    return 1;
}

// Moves all pillars the left with constant speed, if the pillar is completely to the left out of the frame it is "teleported" to the complete right out of the frame
void PillarLogic(PILLAR *pillars, int pillarCount, int frameHeight, int frameWidth, SCORE *score)
{
    for (int i = 0; i < pillarCount; i++)
    {
        (pillars + i)->x--;
        if ((pillars + i)->x == -4) // sa -4 bude vise prostora izmedu svakih (_frameWidth / 20) + 2 pillera pa je sada barem malo lakse za igrati :)
        {
            (pillars + i)->x = frameWidth + 22;
            (pillars + i)->y = 5 + (float)rand() / RAND_MAX * (frameHeight - 10);
        }
        else if ((pillars + i)->x == (frameWidth / 4) - 1)
        {
            score->pillarsPassed++;
        }
    }
}

// Sets pillars in the matrix depening on the location calculate with PillarLogic()
void SetPillars(char **frame, int frameWidth, int frameHeight, PILLAR *pillars, int pillarCount)
{
    for (int i = 0; i < pillarCount; i++)
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

// Updates player physics
void UpdatePlayerPhysics(PLAYER *player, float gravity, int fps)
{
    player->velocity += (gravity / fps);
    player->position += (player->velocity / fps);
}

// Sets player to the frame matrix depening on the physics
void SetPlayer(char **frame, int frameWidth, int frameHeight, PLAYER *player)
{
    if (player->position >= 0 && player->position < frameHeight)
    {
        if (*(*(frame + (int)player->position) + frameWidth / 4) != '#')
        {
            *(*(frame + (int)player->position) + frameWidth / 4) = '@';
        }
        else
        {
            _globalRunning = 0;
        }
    }
    else
    {
        _globalRunning = 0;
    }
}

// Saves players score after the game is finished!
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
    fprintf(fp, "Pillars passed: %d, Spacebar presses: %d, Time alive: %.2f seconds\n", data->pillarsPassed, data->spaceCounter, data->timeSurvived);
    fclose(fp);
}