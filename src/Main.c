#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "..\include\MyFunctions.h"
#include "..\include\MyStructures.h"

const int _fps = 15;
const int _frameWidth = 119;    // Default console width for 16 font size
const int _frameHeight = 29;    // Default console height for 16 font size
const float _gravity = 30;      // Player gravity
const int _horizontalSpeed = 3; // Tiles per second

int main(void)
{
    // Data setup
    srand((unsigned)time(NULL));
    float timeToSleep = (1.0 / _fps) * 1000;   // Time between frames
    int _pillarCount = (_frameWidth / 20) + 2; // Number of pillars
    PLAYER _player = {_frameHeight / 2, 0};    // Player data
    SCORE _playerScore = {0};                  // Player score
    globalRunning = 1;                         // Game loop condition

    // Memory allocation
    PILLAR *_pillars = AllocatePillarMemory(_pillarCount);
    char **_frame = AllocateFrameMemory(_frameWidth, _frameHeight, _pillars);

    // Sets all pillars to the right in the default configuration
    PillarConstructor(_pillars, _pillarCount, _frameWidth, _frameHeight);

    // Console setup
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD startPosition = {0, 0};
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hStdOut, &curInfo);

    // Game loop
    while (globalRunning)
    {
        // Handle the input
        Input(&_player, &_playerScore);

        // Cleares frame data
        ClearFrame(_frame, _frameWidth, _frameHeight);

        // Frame constructors
        PillarLogic(_pillars, _pillarCount, _frameHeight, _frameWidth, &_playerScore); // Generates the pillars
        SetPillars(_frame, _frameWidth, _frameHeight, _pillars, _pillarCount);         // Builds pillas to the frame
        UpdatePlayerPhysics(&_player, _gravity, _fps);                                 // Updates player physics
        SetPlayer(_frame, _frameWidth, _frameHeight, &_player);                        // Builds player to the frame

        // Hides the console cursor, resets the console cursor and displays the frame
        SetConsoleCursorInfo(hStdOut, &curInfo);
        SetConsoleCursorPosition(hStdOut, startPosition);
        DisplayFrame(_frame, _frameHeight); // Renders the frame

        // Sleep to delay the frame
        _playerScore.timeSurvived += 0.033;
        Sleep(timeToSleep); // A rough estimate, since it takes time to render frame it's not perfect but it's ok
    }

    FreeFrameMemory(_frame, _frameHeight);
    free(_pillars);
    SaveUserScore("score.txt", &_playerScore);

    system("cls");
    printf(" Game over! Press ENTER to exit...");
    getchar();
    return 0;
}