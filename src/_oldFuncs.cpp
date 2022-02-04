#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <string.h>


// Saves players score after the game is finished!
void SaveUserScore(SCORE *userScore) {
    // Open file stream
    FILE *fp = fopen("scores.bin", "rb");
    // Check if file exists
    if (fp == NULL) {
        // Generate new if it doesn't exist
        fp = fopen("scores.bin", "wb");
        if (fp == NULL) {
            exit(-1);
        }
        fclose(fp);
    }
    else {
        fclose(fp);
    }
    // Open file for appending
    fp = fopen("scores.bin", "ab");
    if (fp == NULL) {
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
        SetConsoleCursorPosition(hStdOut, { short(frameWidth / 2 - 12), short(frameHeight / 2) });
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
        SetConsoleCursorPosition(hStdOut, { short(frameWidth / 2 - ((strlen((loadedScores + i)->playerName) + 68) / 2)), short(2 + i) });
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
        SetConsoleCursorPosition(hStdOut, { short(frameWidth / 2 - 12), short(frameHeight / 2) });
        printf("No score records found!");
        _getch();
        return;
    }
    fseek(fp, 0, SEEK_END);
    int scoresCounter = ftell(fp) / 30;
    int printCounter = 0;

    // Set cursor to the middle of the screen and let user enter his name
    SetConsoleCursorPosition(hStdOut, { short(frameWidth / 2 - 30), short(frameHeight / 2 - 1) });
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

            SetConsoleCursorPosition(hStdOut, { short(frameWidth / 2 - ((strlen(searchName) + 65) / 2)), short(2 + printCounter) });
            printf("%s:: pillars passed: %d (spacebar presses: %d, time survived: %.2f)", searchName, tempScore.pillarsPassed, tempScore.spaceCounter, tempScore.timeSurvived);
            printCounter++;
        }
    }

    // Print this if a specified player is not found
    if (printCounter == 0) {
        SetConsoleCursorPosition(hStdOut, { short(frameWidth / 2 - 9), short(frameHeight / 2) });
        printf("Player not found!");
    }

    // Close the file stream
    fclose(fp);
    _getch();
}
