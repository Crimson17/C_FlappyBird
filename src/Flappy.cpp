#include "Flappy.h"

#include <iostream>
#include <vector>

#include "Size.h"
#include "Pillar.h"
#include "Frame.h"
#include "Timer.h"


Flappy::Flappy() {
    // Getting the console size
    this->frameSize = this->console.size();

    // Login
    this->login();

    // Main menu
    this->menu();
}

// Login screen
void Flappy::login() {
    // Clearing the console
    this->console.clear();

    // Centering the cursor
    this->console.moveC(this->frameSize.w / 2 - 17, this->frameSize.h / 2 - 1);
    
    // Getting the player name
    printf("Enter your name: ");
    std::string nameInput;
    std::cin >> nameInput;

    // Creating the player
    this->player = Player(nameInput);
}

// Game's main menu
void Flappy::menu() {
    // Menu data
    int chosen = -1;
    int arrowPos = 0;

    // Main menu work
    this->console.clear();
    while (chosen != 4) {
        switch (chosen) {
        case 0:
            // Start the game
            this->game();

            // Data clearing
            chosen = -1;
            this->console.clear();
            break;

        case 1:
            // New player login
            this->login();

            // Data clearing
            chosen = -1;
            this->console.clear();
            break;

        case 2:
            // Sort and print scores
            //HandleScores(frameWidth, frameHeight);
            
            // Data clearing
            chosen = -1;
            this->console.clear();
            break;
        case 3:
            // Search and print specific player
            //SearchForPlayer(frameWidth, frameHeight);

            // Data clearing
            chosen = -1;
            this->console.clear();
            break;

        default:
            // Logged in info
            this->console.moveC(0, 0);
            printf("Logged in as: %s", player.name.c_str());

            // Drawing the logo
            this->console.moveC(this->frameSize.w / 2 - 27, 4);
            printf("    ________                           ____  _          __ ");
            this->console.moveC(this->frameSize.w / 2 - 27, 5);
            printf("   / ____/ /___ _____  ____  __  __   / __ )(_)________/ / ");
            this->console.moveC(this->frameSize.w / 2 - 27, 6);
            printf("  / /_  / / __ `/ __ \\/ __ \\/ / / /  / __  / / ___/ __  /");
            this->console.moveC(this->frameSize.w / 2 - 27, 7);
            printf(" / __/ / / /_/ / /_/ / /_/ / /_/ /  / /_/ / / /  / /_/ /   ");
            this->console.moveC(this->frameSize.w / 2 - 27, 8);
            printf("/_/   /_/\\__,_/ .___/ .___/\\__, /  /_____/_/_/   \\__,_/ ");
            this->console.moveC(this->frameSize.w / 2 - 27, 9);
            printf("             /_/   /_/    /____/                           ");

            // Menu loop
            char keyInput = 0;
            while (keyInput != 13 && keyInput != ' ') {
                // Hide cursor (just in case it becomes visible again)
                this->console.hideC();
                
                // Getting the keyboard input
                keyInput = this->console.input();

                // Checking the keyboard input
                if (keyInput == 72 && arrowPos > 0) {
                    arrowPos--;
                    keyInput = 0;
                }
                else if (keyInput == 80 && arrowPos < 4) {
                    arrowPos++;
                    keyInput = 0;
                }

                // Resetting the menu strings
                std::string menuStrings[5] = {
                    "   Play!   ",
                    "   Relog   ",
                    "   Score   ",
                    "   Find!   ",
                    "   Quit!   "
                };

                // Updating the menu arrows
                menuStrings[arrowPos][0] = '>';
                menuStrings[arrowPos][1] = '>';
                menuStrings[arrowPos][9] = '<';
                menuStrings[arrowPos][10] = '<';

                // Display the menu
                for (int i = 0; i < 5; i++) {
                    this->console.moveC((this->frameSize.w / 2) - 6, (this->frameSize.h / 2) + (i - 1));
                    std::cout << menuStrings[i];
                }
            }

            // Data clearing
            chosen = arrowPos;
            this->console.clear();
            break;
        }
    }
}

// Starts a new game
void Flappy::game() {
    // Calculations
    const float toSleep = 1.0f / 60;

    // Player setup
    this->player.position = this->frameSize.h / 2.0f;
    this->player.velocity = 0;
    this->player.pillarsPassed = 0;
    this->player.spaceCounter = 0;

    // Memory allocation
    std::vector<Pillar> pillars((this->frameSize.w / 20) + 2);
    Frame frame(this->frameSize);

    // Sets all pillars to the right in the default configuration
    for (int i = 0; i < pillars.size(); i++) {
        pillars[i].x = this->frameSize.w + 2 + (i * 20);
        pillars[i].y = rand() % (this->frameSize.h - 15) + 5;
    }

    // Game loop
    console.clear();
    bool running = true;
    Timer timer;
    while (running) {
        // Getting the time values
        const float deltaT = timer.interval();
        const float elapsedT = timer.elapsed();

        // Getting the pressed key
        const char key = this->console.input();

        // Cleares frame data
        frame.clear();

        // Pillar update
        for (int i = 0; i < pillars.size(); i++) {
            player.pillarsPassed += pillars[i].phys(frameSize, deltaT);
            pillars[i].draw(frame, frameSize);
        }

        // Player update
        if (key == ' ') {
            player.velocity -= 10;
            player.spaceCounter++;
        }
        this->player.phys(20, deltaT);
        running = this->player.draw(frame, this->frameSize);

        // Displaying the frame
        this->console.moveC(0, 0);
        frame.display();
    }

    // Saves users score to scores.bin
    //SaveUserScore(&_playerScore);
}
