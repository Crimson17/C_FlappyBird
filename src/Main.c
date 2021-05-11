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

int main(void)
{
    // Data setup
    srand((unsigned)time(NULL));
    char **_frame = AllocateFrameMemory(_frameWidth, _frameHeight);
    float timeToSleep = (1.0 / _fps) * 1000;
    float _playerPosition = _frameHeight / 2;
    float _playerVelocity = 0;
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

    // Game loop
    while (globalRunning)
    {
        // Check for input
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
            }
            else if (c == 27)
            {
                globalRunning = 0;
            }
        }

        // Clears frame data
        ClearFrame(_frame, _frameWidth, _frameHeight);
        SetConsoleCursorInfo(hStdOut, &curInfo);

        // Construct the frame
        UpdatePhysics(&_playerPosition, &_playerVelocity, _gravity, _fps);
        SetPlayerPos(_frame, _frameWidth, _frameHeight, _playerPosition);
        SpawnPillair(_frame, _frameHeight, 90, 5);

        // Display the frame
        DisplayFrame(_frame, _frameHeight);

        // Sleep to delay frame
        Sleep(timeToSleep); // A rough estimate, since it takes time to render frame, it doesn't have to be perfect fps
    }

    FreeFrameMemory(_frame, _frameHeight);
    system("cls");
    printf(" Game over! Press ENTER to exit...");
    getchar();
    return 0;
}