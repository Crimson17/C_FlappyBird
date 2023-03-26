#include "flappy.h"

#include "pillar.h"

#include "console.h"
#include "score.h"
#include "frame.h"
#include "timer.h"

#include <iostream>
#include <vector>


static const std::string scores_file = "scores.bin";

flappy::flappy()
{
    frame_size_ = console::size();
    login();
    menu();
}

void flappy::login()
{
    console::clear();
    console::title("Please login!");
    
    console::move_cursor(frame_size_.width / 2 - 17, frame_size_.height / 2 - 1);
    printf("Enter your name: ");
    std::string name_input = {};
    std::cin >> name_input;

    std::cin.get();
    player_ = player(name_input);
}

void flappy::search() const
{
    console::clear();
    console::move_cursor(frame_size_.width / 2 - 17, frame_size_.height / 2 - 1);

    printf("Name to search: ");
    std::string to_search_for = {};
    std::cin >> to_search_for;

    std::cin.get();
    score::list(scores_file, to_search_for, frame_size_);
}

void flappy::menu()
{
    int chosen = -1;
    int arrow_pos = 0;

    console::clear();
    while (chosen != 4) {
        switch (chosen) {
        case 0:
            game();

            chosen = -1;
            console::clear();
            break;

        case 1:
            login();
            
            chosen = -1;
            console::clear();
            break;

        case 2:
            score::list(scores_file, frame_size_);
            
            chosen = -1;
            console::clear();
            break;

        case 3:
            search();
            
            chosen = -1;
            console::clear();
            break;

        default:
            console::title("Flappy!");

            console::move_cursor(0, 0);
            printf("Logged in as: %s", player_.name.c_str());

            console::move_cursor(frame_size_.width / 2 - 27, 4);
            printf("    ________                           ____  _          __ ");
            console::move_cursor(frame_size_.width / 2 - 27, 5);
            printf("   / ____/ /___ _____  ____  __  __   / __ )(_)________/ / ");
            console::move_cursor(frame_size_.width / 2 - 27, 6);
            printf("  / /_  / / __ `/ __ \\/ __ \\/ / / /  / __  / / ___/ __  /");
            console::move_cursor(frame_size_.width / 2 - 27, 7);
            printf(" / __/ / / /_/ / /_/ / /_/ / /_/ /  / /_/ / / /  / /_/ /   ");
            console::move_cursor(frame_size_.width / 2 - 27, 8);
            printf("/_/   /_/\\__,_/ .___/ .___/\\__, /  /_____/_/_/   \\__,_/ ");
            console::move_cursor(frame_size_.width / 2 - 27, 9);
            printf("             /_/   /_/    /____/                           ");

            char key_input = 0;
            while (key_input != 13 && key_input != ' ') {
                console::hide_cursor();
                
                key_input = console::read_input();
                if (key_input == 72 && arrow_pos > 0) {
                    arrow_pos--;
                    key_input = 0;
                }
                else if (key_input == 80 && arrow_pos < 4) {
                    arrow_pos++;
                    key_input = 0;
                }

                std::string menu_strings[5] = {
                    "   Play!   ",
                    "   Relog   ",
                    "   Score   ",
                    "   Find!   ",
                    "   Quit!   "
                };

                menu_strings[arrow_pos][0] = '>';
                menu_strings[arrow_pos][1] = '>';
                menu_strings[arrow_pos][9] = '<';
                menu_strings[arrow_pos][10] = '<';

                for (int i = 0; i < 5; i++) {
                    console::move_cursor((frame_size_.width / 2) - 6, (frame_size_.height / 2) + (i - 1));
                    std::cout << menu_strings[i];
                }
            }

            chosen = arrow_pos;
            console::clear();
            break;
        }
    }
}

void flappy::game()
{
    player_.position = frame_size_.height * 0.5f;
    player_.velocity = 0;

    score score(player_.name);

    std::vector<pillar> pillars((frame_size_.width / 20) + 2);
    frame frame(frame_size_);

    for (uint64_t i = 0; i < pillars.size(); i++) {
        pillars[i].x = frame_size_.width + 2.0f + (i * 20.0f);
        pillars[i].y = (rand() % (frame_size_.height - 15)) + 5.0f;
    }

    console::clear();
    bool running = true;
    timer timer;
    
    while (running) {
        const float delta_t = timer.get_interval();
        const char key = console::read_input();
        frame.clear();

        for (auto& pillar : pillars) {
            score.pillars += pillar.phys(frame_size_, delta_t);
            pillar.draw(frame, frame_size_);
        }

        if (key == ' ') {
            player_.velocity -= 10;
            score.spaces++;
        }
        player_.phys_update(20, delta_t);
        running = player_.draw(frame, frame_size_);

        console::move_cursor(0, 0);
        frame.display();
    }
    score.time = timer.get_elapsed();

    score.save(scores_file);
}
