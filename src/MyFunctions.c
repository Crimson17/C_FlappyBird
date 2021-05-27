//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include "../include/MyFunctions.h"
#include "../include/MyStructures.h"

void Menu(const int frameWidth, const int frameHeight, const int fps, const float gravity)
{
    int menuDecision = -1;
    int menuCursor = 0;
    char keyboardInput = 0;
    char menuString[5][13];

    // Start loop until spacebar or enter
    system("cls");
    while (menuDecision != 4)
    {
        switch (menuDecision)
        {
        case 0:
            // Start the game
            Game(frameWidth, frameHeight, fps, gravity);
            menuDecision = -1;
            system("cls");
            break;
        case 1:
            // Load new player name
            NewPlayer(frameWidth, frameHeight);
            menuDecision = -1;
            system("cls");
            break;
        case 2:
            // Sort and print scores
            HandleScores(frameWidth, frameHeight);
            menuDecision = -1;
            system("cls");
            break;
        case 3:
            // Search and print specific player
            SearchForPlayer(frameWidth, frameHeight);
            menuDecision = -1;
            system("cls");
            break;
        default:
            // Show who's logged in
            SetConsoleCursorPosition(hStdOut, (COORD){0, 0});
            printf("Logged in as: %s", _playerName);
            /* The most important part... */
            SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - 27, 4});
            printf("    ________                           ____  _          __ ");
            SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - 27, 5});
            printf("   / ____/ /___ _____  ____  __  __   / __ )(_)________/ / ");
            SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - 27, 6});
            printf("  / /_  / / __ `/ __ \\/ __ \\/ / / /  / __  / / ___/ __  /");
            SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - 27, 7});
            printf(" / __/ / / /_/ / /_/ / /_/ / /_/ /  / /_/ / / /  / /_/ /   ");
            SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - 27, 8});
            printf("/_/   /_/\\__,_/ .___/ .___/\\__, /  /_____/_/_/   \\__,_/ ");
            SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - 27, 9});
            printf("             /_/   /_/    /____/                           ");
            /* drawing the logo ofc ;) */

            // Loop until the user chooses an option
            keyboardInput = 0;
            while (keyboardInput != 13 && keyboardInput != 32)
            {
                // Hide cursor (just in case it becomes visible again)
                SetConsoleCursorInfo(hStdOut, &curInfo);

                // Get keyboard input while chars are stored in the buffer
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
                else if (keyboardInput == 80 && menuCursor < 4)
                {
                    menuCursor++;
                    keyboardInput = 0;
                }

                // Reset the menu strings
                strcpy(menuString[0], "   Play!   \n");
                strcpy(menuString[1], "   Relog   \n");
                strcpy(menuString[2], "   Score   \n");
                strcpy(menuString[3], "   Find!   \n");
                strcpy(menuString[4], "   Quit!   \n");

                // Update the menu strings
                menuString[menuCursor][0] = '>';
                menuString[menuCursor][1] = '>';
                menuString[menuCursor][9] = '<';
                menuString[menuCursor][10] = '<';

                // Display the menu
                for (int i = 0; i < 5; i++)
                {
                    SetConsoleCursorPosition(hStdOut, (COORD){(frameWidth / 2) - 6, (frameHeight / 2) + (i - 1)});
                    printf("%s", menuString[i]);
                }
            }
            menuDecision = menuCursor;
            system("cls");
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

    // Game loop
    system("cls");
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
        SetConsoleCursorPosition(hStdOut, (COORD){0, 0});
        DisplayFrame(_frame, frameHeight);

        // Sleep to delay the frame
        _playerScore.timeSurvived += 1.0 / fps;
        Sleep(_timeToSleep); // A rough estimate, since it takes time to render frame it's not perfect but it's ok
    }

    // Free memory
    FreeFrameMemory(_frame, frameHeight);
    free(_pillars);

    // Saves users score to scores.bin
    SaveUserScore(&_playerScore);
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

// Let user enter his/her name
void NewPlayer(int frameWidth, int frameHeight)
{
    system("cls");

    // Set cursor to the middle of the screen and let user enter his name
    SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - 17, frameHeight / 2 - 1});
    printf("Enter your name: ");
    fgets(_playerName, 18, stdin);
    // Removing \n
    for (int i = 0; *(_playerName + i) != '\0'; i++)
    {
        if (*(_playerName + i) == '\n')
        {
            *(_playerName + i) = '\0';
            break;
        }
    }
}

// Saves players score after the game is finished!
void SaveUserScore(SCORE *userScore)
{
    // Open file stream
    FILE *fp = fopen("scores.bin", "rb");
    // Check if file exists
    if (fp == NULL)
    {
        // Generate new if it doesn't exist
        fp = fopen("scores.bin", "wb");
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
    fp = fopen("scores.bin", "ab");
    if (fp == NULL)
    {
        exit(-1);
    }

    // Move to the end of the file
    fseek(fp, 0, SEEK_END);
    // Save player name
    fwrite(_playerName, sizeof(char), 18, fp);
    // Save pillars passed
    fwrite(&userScore->pillarsPassed, sizeof(int), 1, fp);
    // Save spacebar presses
    fwrite(&userScore->spaceCounter, sizeof(int), 1, fp);
    // Save time survived
    fwrite(&userScore->timeSurvived, sizeof(float), 1, fp);
    // Close the file stream
    fclose(fp);
}

void HandleScores(int frameWidth, int frameHeight)
{
    // Count scores
    FILE *fp = fopen("scores.bin", "rb");
    if (fp == NULL)
    {
        SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - 12, frameHeight / 2});
        printf("No score records found!");
        _getch();
        return;
    }
    fseek(fp, 0, SEEK_END);
    int scoresCounter = ftell(fp) / 30;

    // Allocate memory
    SCORE *loadedScores = (SCORE *)calloc(scoresCounter, sizeof(SCORE));
    if (loadedScores == NULL)
    {
        fclose(fp);
        exit(-1);
    }

    // Read from file to temp memory
    for (int i = 0; i < scoresCounter; i++)
    {
        fseek(fp, 30 * i, SEEK_SET);
        fread((loadedScores + i)->playerName, sizeof(char), 18, fp);
        fseek(fp, 30 * i + 18, SEEK_SET);
        fread(&(loadedScores + i)->pillarsPassed, sizeof(int), 1, fp);
        fseek(fp, 30 * i + 22, SEEK_SET);
        fread(&(loadedScores + i)->spaceCounter, sizeof(int), 1, fp);
        fseek(fp, 30 * i + 26, SEEK_SET);
        fread(&(loadedScores + i)->timeSurvived, sizeof(float), 1, fp);
    }
    fclose(fp);

    // Sort scores by pillars passed, if they are the same then sort by spacebar presses
    for (int i = 0; i < scoresCounter; i++)
    {
        for (int j = i; j < scoresCounter; j++)
        {
            if ((loadedScores + j)->pillarsPassed > (loadedScores + i)->pillarsPassed)
            {
                // Replace
                char tempName[18];
                strcpy(tempName, (loadedScores + i)->playerName);
                int tempPillars = (loadedScores + i)->pillarsPassed;
                int tempSpaces = (loadedScores + i)->spaceCounter;
                float tempTime = (loadedScores + i)->timeSurvived;

                strcpy((loadedScores + i)->playerName, (loadedScores + j)->playerName);
                (loadedScores + i)->pillarsPassed = (loadedScores + j)->pillarsPassed;
                (loadedScores + i)->spaceCounter = (loadedScores + j)->spaceCounter;
                (loadedScores + i)->timeSurvived = (loadedScores + j)->timeSurvived;

                strcpy((loadedScores + j)->playerName, tempName);
                (loadedScores + j)->pillarsPassed = tempPillars;
                (loadedScores + j)->spaceCounter = tempSpaces;
                (loadedScores + j)->timeSurvived = tempTime;
            }
            else if ((loadedScores + j)->pillarsPassed == (loadedScores + i)->pillarsPassed && (loadedScores + j)->spaceCounter < (loadedScores + i)->spaceCounter)
            {
                // Replace
                char tempName[18];
                strcpy(tempName, (loadedScores + i)->playerName);
                int tempPillars = (loadedScores + i)->pillarsPassed;
                int tempSpaces = (loadedScores + i)->spaceCounter;
                float tempTime = (loadedScores + i)->timeSurvived;

                strcpy((loadedScores + i)->playerName, (loadedScores + j)->playerName);
                (loadedScores + i)->pillarsPassed = (loadedScores + j)->pillarsPassed;
                (loadedScores + i)->spaceCounter = (loadedScores + j)->spaceCounter;
                (loadedScores + i)->timeSurvived = (loadedScores + j)->timeSurvived;

                strcpy((loadedScores + j)->playerName, tempName);
                (loadedScores + j)->pillarsPassed = tempPillars;
                (loadedScores + j)->spaceCounter = tempSpaces;
                (loadedScores + j)->timeSurvived = tempTime;
            }
        }
    }

    // Print scores
    system("cls");
    for (int i = 0; i < scoresCounter; i++)
    {
        SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - ((strlen((loadedScores + i)->playerName) + 68) / 2), 2 + i});
        printf("%d. %s:: pillars passed: %d (spacebar presses: %d, time survived: %.2f)", i + 1, (loadedScores + i)->playerName, (loadedScores + i)->pillarsPassed, (loadedScores + i)->spaceCounter, (loadedScores + i)->timeSurvived);
    }

    // Free scores memory
    free(loadedScores);
    _getch();
}

void SearchForPlayer(int frameWidth, int frameHeight)
{
    char searchName[18];
    char tempName[18];

    // Count scores
    FILE *fp = fopen("scores.bin", "rb");
    if (fp == NULL)
    {
        SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - 12, frameHeight / 2});
        printf("No score records found!");
        _getch();
        return;
    }
    fseek(fp, 0, SEEK_END);
    int scoresCounter = ftell(fp) / 30;
    int printCounter = 0;

    // Set cursor to the middle of the screen and let user enter his name
    SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - 30, frameHeight / 2 - 1});
    printf("Enter the name to search for: ");
    fgets(searchName, 18, stdin);
    // Removing \n
    for (int i = 0; *(searchName + i) != '\0'; i++)
    {
        if (*(searchName + i) == '\n')
        {
            *(searchName + i) = '\0';
            break;
        }
    }

    // Go through bin file and print if the names match
    system("cls");
    for (int i = 0; i < scoresCounter; i++)
    {
        fseek(fp, 30 * i, SEEK_SET);
        fread(tempName, sizeof(char), 18, fp);

        if (!strcmp(tempName, searchName))
        {
            SCORE tempScore = {0};
            fseek(fp, 30 * i + 18, SEEK_SET);
            fread(&tempScore.pillarsPassed, sizeof(int), 1, fp);
            fseek(fp, 30 * i + 22, SEEK_SET);
            fread(&tempScore.spaceCounter, sizeof(int), 1, fp);
            fseek(fp, 30 * i + 26, SEEK_SET);
            fread(&tempScore.timeSurvived, sizeof(float), 1, fp);

            SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - ((strlen(searchName) + 65) / 2), 2 + printCounter});
            printf("%s:: pillars passed: %d (spacebar presses: %d, time survived: %.2f)", searchName, tempScore.pillarsPassed, tempScore.spaceCounter, tempScore.timeSurvived);
            printCounter++;
        }
    }
    // Print this if a specified player is not found
    if (printCounter == 0)
    {
        SetConsoleCursorPosition(hStdOut, (COORD){frameWidth / 2 - 9, frameHeight / 2});
        printf("Player not found!");
    }
    // Close the file stream
    fclose(fp);
    _getch();
}
