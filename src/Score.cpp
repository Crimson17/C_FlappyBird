#include "Score.h"

#include <iostream>
#include <sstream>

#include "Console.h"


Score::Score() {
	memset(this->name, 0, sizeof(this->name));
	this->spaces = 0;
	this->pillars = 0;
	this->time = 0;
}

Score::Score(const std::string& name) {
	memset(this->name, 0, sizeof(this->name));
	memcpy(this->name, &name[0], min(name.size(), sizeof(this->name)));
	this->spaces = 0;
	this->pillars = 0;
	this->time = 0;
}

// Saves the score to a file
void Score::save(const std::string& filePath) {
	// Opening the file
	FILE* file = fopen(filePath.c_str(), "ab");

	// Saving the data
	fwrite(this, sizeof(*this), 1, file);

	// Closing the file
	fclose(file);
}

// Reads and returns the scores
std::vector<Score> Score::read(const std::string& filePath) {
    // Opening the file
    FILE* file = fopen(filePath.c_str(), "rb");
    if (!file) {
        return std::vector<Score>();
    }

    // Getting the file size
    fseek(file, 0, SEEK_END);
    const int byteSize = ftell(file);
    const int scoreCount = byteSize / sizeof(Score);

    // Score buffer creation
    std::vector<Score> scores(scoreCount);

    // Reading the scores to the buffer
    rewind(file);
    fread(&scores[0], sizeof(Score), scores.size(), file);

    // Closing the file
    fclose(file);

    // Returning
    return scores;
}

// Sorts and lists all players
void Score::list(const std::string& filePath, const Size& frameSize) {
    // Reading the scores
    std::vector<Score> scores = Score::read(filePath);

    // Checking the vector size
    if (scores.size() == 0) {
        return;
    }

    // Sorting the scores
    for (int i = 0; i < scores.size(); i++) {
        for (int j = i; j < scores.size(); j++) {
            if ((scores[j].pillars > scores[i].pillars) || (scores[j].pillars == scores[i].pillars && scores[j].spaces < scores[i].spaces)) {
                Score temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }

    // Printing the scores
    Console::clear();
    for (int i = 0; i < scores.size(); i++) {
        // Generating the string
        std::stringstream ss;
        ss << (i + 1) << ". " << scores[i].name << ":: pillars : " << scores[i].pillars << " | spacebars: " << scores[i].spaces << " | time: " << scores[i].time;
        const std::string toPrint = ss.str();

        // Aligning the cursor
        Console::moveC(frameSize.w / 2 - int(toPrint.size()) / 2, 2 + i);

        // Printing
        std::cout << toPrint;
    }
    std::cin.get();
}

// Lists the player scores
void Score::list(const std::string& filePath, const std::string& name, const Size& frameSize) {
    // Reading the scores
    std::vector<Score> scores = Score::read(filePath);

    // Checking the vector size
    if (scores.size() == 0) {
        return;
    }

    // Removing the other players
    std::vector<Score> temp;
    for (int i = 0; i < scores.size(); i++) {
        if (std::string(scores[i].name) == name) {
            temp.push_back(scores[i]);
        }
    }
    scores = temp;

    // Checking if there are no scores
    Console::clear();
    if (scores.size() == 0) {
        // Message
        std::string message = "There are NO available scores for this player!";

        // Aligning the cursor
        Console::moveC(frameSize.w / 2 - int(message.size()) / 2, frameSize.h / 2 - 2);

        // Printing
        std::cout << message;
    }
    // Printing the scores
    else {
        for (int i = 0; i < scores.size(); i++) {
            // Generating the string
            std::stringstream ss;
            ss << (i + 1) << ". " << scores[i].name << ":: pillars : " << scores[i].pillars << " | spacebars: " << scores[i].spaces << " | time: " << scores[i].time;
            const std::string toPrint = ss.str();

            // Aligning the cursor
            Console::moveC(frameSize.w / 2 - int(toPrint.size()) / 2, 2 + i);

            // Printing
            std::cout << toPrint;
        }
    }
    std::cin.get();
}
