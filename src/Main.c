#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include "..\include\MyFunctions.h"
#include "..\include\MyStructures.h"

const int _fps = 15;
const int _frameWidth = 119; // Default console width for 16 font size
const int _frameHeight = 29; // Default console height for 16 font size
const float _gravity = 30;
const int _horizontalSpeed = 3; // Tiles per second

int main(void)
{
    // Memory allocation
    char **_frame = AllocateFrameMemory(_frameWidth, _frameHeight);
    PILLAR *pillars = (PILLAR *)calloc((_frameWidth / 20) + 2, sizeof(PILLAR));
    if (pillars == NULL)
    {
        return -1;
    }

    // Data setup
    srand((unsigned)time(NULL));
    float timeToSleep = (1.0 / _fps) * 1000;

    float _playerPosition = _frameHeight / 2; // Should turn this into a struct called player
    float _playerVelocity = 0;

    SCORE playerScore = {0};
    int pillarSetupCounter = 0;
    for (int i = 0; i < (_frameWidth / 20) + 2; i++)
    {
        (pillars + i)->x = _frameWidth + 2 + pillarSetupCounter;
        (pillars + i)->y = 5 + (float)rand() / RAND_MAX * (_frameHeight - 10);
        pillarSetupCounter += 20;
    }
    globalRunning = 1;

    // Console setup
    HANDLE hStdout;
    COORD destCoord;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    destCoord.X = 0;
    destCoord.Y = 0;
    HANDLE hStdOut = NULL;
    CONSOLE_CURSOR_INFO curInfo;
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hStdOut, &curInfo);

    int frameCounter = 0;
    // Game loop
    while (globalRunning)
    {
        // Handle the input
        if (kbhit())
        {
            char c;
            while (kbhit())
            {
                c = _getch();
            }
            if (c == 32)
            {
                _playerVelocity -= 10;
                playerScore.spaceCounter++;
            }
            else if (c == 27)
            {
                globalRunning = 0;
            }
        }

        // Cleares frame data
        ClearFrame(_frame, _frameWidth, _frameHeight);
        SetConsoleCursorInfo(hStdOut, &curInfo);

        // Constructs the frame
        PillarLogic(pillars, (_frameWidth / 20) + 2, frameCounter, _frameHeight, _frameWidth);
        SetPillars(_frame, _frameWidth, _frameHeight, pillars, (_frameWidth / 20) + 2);
        UpdatePhysics(&_playerPosition, &_playerVelocity, _gravity, _fps);
        SetPlayer(_frame, _frameWidth, _frameHeight, _playerPosition);

        // Displays the frame
        DisplayFrame(_frame, _frameHeight);

        // Sleep to delay the frame
        frameCounter++;
        playerScore.timeSurvived += 0.033;
        Sleep(timeToSleep); // A rough estimate, since it takes time to render frame it's not perfect but it's ok
    }

    FreeFrameMemory(_frame, _frameHeight);
    free(pillars);
    SaveUserScore("score.txt", &playerScore);

    system("cls");
    printf(" Game over! Press ENTER to exit...");
    getchar();
    return 0;
}