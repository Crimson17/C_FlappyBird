#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/MyFunctions.h"
#include "../include/MyStructures.h"

// Game properties
const int _frameWidth = 119; // Default console width for 16 font size (shouldn't be less than this default value)
const int _frameHeight = 29; // Default console height for 16 font size (shouldn't be less than this default value)
const int _fps = 15;         // Game fps (15 actually works really good)
const float _gravity = 30;   // Player gravity (10 is too slow)

int main(void)
{
    srand((unsigned)time(NULL));

    Menu(_frameWidth, _frameHeight, _fps, _gravity);

    return 0;
}