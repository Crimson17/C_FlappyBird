#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include "../include/MyFunctions.h"
#include "../include/MyStructures.h"

// Game properties
const int _fps = 15;
const int _frameWidth = 119; // Default console width for 16 font size
const int _frameHeight = 29; // Default console height for 16 font size
const float _gravity = 30;   // Player gravity

int main(void)
{
    srand((unsigned)time(NULL));
    int menuInput = 0;

    while (menuInput != 3)
    {
        switch (menuInput)
        {
        case 1:
            // ChoosePlayer();
            Game(_frameWidth, _frameHeight, _fps, _gravity);
            menuInput = 0;
            break;
        case 2:
            // SortScores();
            // PrintScores();
            menuInput = 0;
            break;
        default:
            system("cls");
            printf("1. Play\n2. Scores\n3. Quit\n");
            scanf("%d", &menuInput);
            break;
        }
    }

    return 0;
}