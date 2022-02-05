#include "Flappy.h"

#include <iostream>
#include <vector>

#include "Pillar.h"
#include "Score.h"
#include "Frame.h"
#include "Timer.h"


static const std::string scoresFile = "scores.bin";

Flappy::Flappy() {
    // Getting the console size
    this->frameSize = Console::size();

    // Login
    this->login();

    // Main menu
    this->menu();
}

// Login screen
void Flappy::login() {
    // Clearing the console
    Console::clear();

    // Title update
    Console::title("Please login!");

    // Centering the cursor
    Console::moveC(this->frameSize.w / 2 - 17, this->frameSize.h / 2 - 1);
    
    // Getting the player name
    printf("Enter your name: ");
    std::string nameInput;
    std::cin >> nameInput;

    // Flushing the input
    std::cin.get();

    // Creating the player
    this->player = Player(nameInput);
}

// Prints the specific player scores
void Flappy::search() {
    // Clearing the console
    Console::clear();

    // Centering the cursor
    Console::moveC(this->frameSize.w / 2 - 17, this->frameSize.h / 2 - 1);

    // Getting the name
    printf("Name to search: ");
    std::string toSearchFor;
    std::cin >> toSearchFor;

    // Flushing the input
    std::cin.get();

    // Search and print specific player scores
    Score::list(scoresFile, toSearchFor, frameSize);
}

// Game's main menu
void Flappy::menu() {
    // Menu data
    int chosen = -1;
    int arrowPos = 0;

    // Main menu work
    Console::clear();
    while (chosen != 4) {
        switch (chosen) {
        case 0:
            // Start the game
            this->game();

            // Data clearing
            chosen = -1;
            Console::clear();
            break;

        case 1:
            // New player login
            this->login();

            // Data clearing
            chosen = -1;
            Console::clear();
            break;

        case 2:
            // Sort and print scores
            Score::list(scoresFile, this->frameSize);
            
            // Data clearing
            chosen = -1;
            Console::clear();
            break;

        case 3:
            // Search and print specific player scores
            this->search();

            // Data clearing
            chosen = -1;
            Console::clear();
            break;

        default:
            // Title update
            Console::title("Flappy!");

            // Logged in info
            Console::moveC(0, 0);
            printf("Logged in as: %s", player.name.c_str());

            // Drawing the logo
            Console::moveC(this->frameSize.w / 2 - 27, 4);
            printf("    ________                           ____  _          __ ");
            Console::moveC(this->frameSize.w / 2 - 27, 5);
            printf("   / ____/ /___ _____  ____  __  __   / __ )(_)________/ / ");
            Console::moveC(this->frameSize.w / 2 - 27, 6);
            printf("  / /_  / / __ `/ __ \\/ __ \\/ / / /  / __  / / ___/ __  /");
            Console::moveC(this->frameSize.w / 2 - 27, 7);
            printf(" / __/ / / /_/ / /_/ / /_/ / /_/ /  / /_/ / / /  / /_/ /   ");
            Console::moveC(this->frameSize.w / 2 - 27, 8);
            printf("/_/   /_/\\__,_/ .___/ .___/\\__, /  /_____/_/_/   \\__,_/ ");
            Console::moveC(this->frameSize.w / 2 - 27, 9);
            printf("             /_/   /_/    /____/                           ");

            // Menu loop
            char keyInput = 0;
            while (keyInput != 13 && keyInput != ' ') {
                // Hide cursor (just in case it becomes visible again)
                Console::hideC();
                
                // Getting the keyboard input
                keyInput = Console::input();

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
                    Console::moveC((this->frameSize.w / 2) - 6, (this->frameSize.h / 2) + (i - 1));
                    std::cout << menuStrings[i];
                }
            }

            // Data clearing
            chosen = arrowPos;
            Console::clear();
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

    // Score setup
    Score score(this->player.name);

    // Memory allocation
    std::vector<Pillar> pillars((this->frameSize.w / 20) + 2);
    Frame frame(this->frameSize);

    // Sets all pillars to the right in the default configuration
    for (int i = 0; i < pillars.size(); i++) {
        pillars[i].x = this->frameSize.w + 2.0f + (i * 20);
        pillars[i].y = rand() % (this->frameSize.h - 15) + 5.0f;
    }

    // Game loop
    Console::clear();
    bool running = true;
    Timer timer;
    while (running) {
        // Getting the time values
        const float deltaT = timer.interval();
        const float elapsedT = timer.elapsed();

        // Getting the pressed key
        const char key = Console::input();

        // Cleares frame data
        frame.clear();

        // Pillar update
        for (int i = 0; i < pillars.size(); i++) {
            score.pillars += pillars[i].phys(frameSize, deltaT);
            pillars[i].draw(frame, frameSize);
        }

        // Player update
        if (key == ' ') {
            player.velocity -= 10;
            score.spaces++;
        }
        this->player.phys(20, deltaT);
        running = this->player.draw(frame, this->frameSize);

        // Displaying the frame
        Console::moveC(0, 0);
        frame.display();
    }
    score.time = timer.elapsed();

    // Saving the score to a bin file
    score.save(scoresFile);
}
