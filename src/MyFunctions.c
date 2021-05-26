#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <windows.h>
#include "../include/MyFunctions.h"
#include "../include/MyStructures.h"

void Menu(const int frameWidth, const int frameHeight, const int fps, const float gravity){
    int menuDecision = -1;
    int menuCursor = 0;
    char keyboardInput = 0;
    char menuString[3][13];

    // Console setup
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hStdOut, &curInfo);

    while (menuDecision != 2)
    {
        switch (menuDecision)
        {
        case 0:
            // ChoosePlayer();
            Game(frameWidth, frameHeight, fps, gravity);
            menuDecision = -1;
            system("cls");
            break;
        case 1:
            // SortScores();
            // PrintScores();
            menuDecision = -1;
            break;
        default:
            keyboardInput = 0;

            while (keyboardInput != 13)
            {
                // Hide cursor (just in case it becomes visible again)
                SetConsoleCursorInfo(hStdOut, &curInfo);

                // Get keyboard input
                while (_kbhit())
                {
                    keyboardInput = _getch();
                }

                // Check keyboard input
                if (keyboardInput == 72 && menuCursor > 0)
                {
                    menuCursor--;
                    keyboardInput = 0;
                }
                else if (keyboardInput == 80 && menuCursor < 2)
                {
                    menuCursor++;
                    keyboardInput = 0;
                }

                // Reset the menu strings
                strcpy(menuString[0], "   Play!   \n");
                strcpy(menuString[1], "   Score   \n");
                strcpy(menuString[2], "   Quit!   \n");

                // Update the menu strings
                menuString[menuCursor][0] = '>';
                menuString[menuCursor][1] = '>';
                menuString[menuCursor][9] = '<';
                menuString[menuCursor][10] = '<';

                // Display the menu
                SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - 6, frameHeight / 2 - 1});
                printf("%s", menuString[0]);
                SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - 6, frameHeight / 2});
                printf("%s", menuString[1]);
                SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - 6, frameHeight / 2 + 1});
                printf("%s", menuString[2]);
            }
            menuDecision = menuCursor;
            break;
        }
    }
}

void Game(const int frameWidth, const int frameHeight, const int fps, const float gravity)
{
    // Data setup
    float _timeToSleep = (1.0 / fps) * 1000;  // Time between frames
    int _pillarCount = (frameWidth / 20) + 2; // Number of pillars
    PLAYER _player = {frameHeight / 2, 0};    // Player data
    SCORE _playerScore = {0};                 // Player score
    _globalRunning = 1;                       // Game loop condition

    // Memory allocation
    PILLAR *_pillars = AllocatePillarMemory(_pillarCount);
    char **_frame = AllocateFrameMemory(frameWidth, frameHeight, _pillars);

    // Sets all pillars to the right in the default configuration
    PillarConstructor(_pillars, _pillarCount, frameWidth, frameHeight);

    // Console setup
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD startPosition = {0, 0};
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hStdOut, &curInfo);

    // Prepare screen for rendering
    system("cls");

    // Game loop
    while (_globalRunning)
    {
        // Handle the input
        Input(&_player, &_playerScore);

        // Cleares frame data
        ClearFrame(_frame, frameWidth, frameHeight);

        // Frame constructors
        PillarLogic(_pillars, _pillarCount, frameHeight, frameWidth, &_playerScore); // Generates the pillars
        SetPillars(_frame, frameWidth, frameHeight, _pillars, _pillarCount);         // Builds pillas to the frame
        UpdatePlayerPhysics(&_player, gravity, fps);                                 // Updates player physics
        SetPlayer(_frame, frameWidth, frameHeight, &_player);                        // Builds player to the frame

        // Hides the console cursor, resets the console cursor and displays the frame
        SetConsoleCursorInfo(hStdOut, &curInfo);
        SetConsoleCursorPosition(hStdOut, startPosition);
        DisplayFrame(_frame, frameHeight);

        // Sleep to delay the frame
        _playerScore.timeSurvived += 1.0 / fps;
        Sleep(_timeToSleep); // A rough estimate, since it takes time to render frame it's not perfect but it's ok
    }

    // Free memory
    FreeFrameMemory(_frame, frameHeight);
    free(_pillars);

    // Saves users score to score.txt
    SaveUserScore("score.txt", &_playerScore);
}

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

// Default pillar configuration, places all pillars to the left of the screen for the first time
void PillarConstructor(PILLAR *pillars, int pillarCount, int frameWidth, int frameHeight)
{
    for (int i = 0; i < pillarCount; i++)
    {
        (pillars + i)->x = frameWidth + 2 + (i * 20);
        (pillars + i)->y = 5 + (float)rand() / RAND_MAX * (frameHeight - 10);
    }
}

// Handles the keyboard input (SPACEBAR)
void Input(PLAYER *player, SCORE *score)
{
    if (_kbhit())
    {
        char input = '\0';
        while (_kbhit())
        {
            input = _getch();
        }
        if (input == 32)
        {
            player->velocity -= 10;
            score->spaceCounter++;
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
        // Pillar movement
        (pillars + i)->x--;
        // Pillar teleportation
        if ((pillars + i)->x == -4)
        {
            (pillars + i)->x = frameWidth + 22;
            (pillars + i)->y = 5 + (float)rand() / RAND_MAX * (frameHeight - 10);
        }
        // Check if the player has passed a pillar
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
        // Generates upper pillar
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
        // Generates lower pillar
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
    // Check if player is in the frame, else finish the game
    if (player->position >= 0 && player->position < frameHeight)
    {
        // Check if player is not in the '#', else finish the game
        if (*(*(frame + (int)player->position) + frameWidth / 4) != '#')
        {
            // If the player is in the allowed position than draw him to the frame
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
    // Open file stream
    FILE *fp = fopen(fileName, "r");
    // Check if file exists
    if (fp == NULL)
    {
        // Generate new if it doesn't exist
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
    // Open file for appending
    fp = fopen(fileName, "a");
    if (fp == NULL)
    {
        exit(-1);
    }
    // Save score to file and close the file stream
    fprintf(fp, "Pillars passed: %d, Spacebar presses: %d, Time alive: %.2f seconds\n", data->pillarsPassed, data->spaceCounter, data->timeSurvived);
    fclose(fp);
}