#include "score.h"
#include "console.h"

#include <iostream>
#include <sstream>


score::score()
{}

score::score(const std::string& name)
{
	memcpy(this->name, name.data(), min(name.size(), sizeof(this->name)));
}

void score::save(const std::string& filepath) const
{
    if (FILE* file = nullptr; !fopen_s(&file, filepath.c_str(), "ab")) {
        fwrite(this, sizeof(*this), 1, file);
        fclose(file);
    }
    else {
        std::cout << "File \"" << filepath << "\" error" << std::endl;
    }
}

std::vector<score> score::load(const std::string& filepath)
{
    FILE* file = nullptr;
    if (fopen_s(&file, filepath.c_str(), "rb")) {
        std::cout << "File \"" << filepath << "\" error" << std::endl;
        return {};
    }

    fseek(file, 0, SEEK_END);
    const int byte_size = ftell(file);
    rewind(file);
    
    std::vector<score> scores(byte_size / sizeof(score));
    fread(scores.data(), sizeof(score), scores.size(), file);
    fclose(file);

    return scores;
}

void score::list(const std::string& filepath, const size& frame_size)
{
    std::vector<score> scores = load(filepath);
    if (scores.empty()) {
        return;
    }

    for (uint64_t i = 0; i < scores.size(); i++) {
        for (uint64_t j = i; j < scores.size(); j++) {
            if ((scores[j].pillars > scores[i].pillars) || (scores[j].pillars == scores[i].pillars && scores[j].spaces < scores[i].spaces)) {
                std::swap(scores[i], scores[j]);
            }
        }
    }

    int longest_name = 0;
    for (auto& score : scores) {
        if (const int temp_len = static_cast<int>(std::string(score.name).size()); temp_len > longest_name) {
            longest_name = temp_len;
        }
    }
    longest_name += 1;

    console::clear();
    for (uint64_t i = 0; i < scores.size(); i++) {
        std::stringstream stream;
        stream.width(5);
        stream << (i + 1);
        stream << ". ";
        stream.width(longest_name);
        stream << scores[i].name;
        stream << " -> pillars : ";
        stream.width(5);
        stream << scores[i].pillars;
        stream << " | spacebars: ";
        stream.width(5);
        stream << scores[i].spaces;
        stream << " | time: ";
        stream.width(8);
        stream.precision(2);
        stream << std::fixed;
        stream << scores[i].time;
        const std::string to_print = stream.str();
        
        console::move_cursor(frame_size.width / 2 - static_cast<int>(to_print.size()) / 2, static_cast<int>(2 + i));
        std::cout << to_print;
    }
    std::cin.get();
}

void score::list(const std::string& filepath, const std::string& name, const size& frame_size)
{
    std::vector<score> scores = load(filepath);
    if (scores.empty()) {
        return;
    }

    std::vector<score> temp = {};
    for (auto& score : scores) {
        if (std::string(score.name) == name) {
            temp.push_back(score);
        }
    }
    scores = temp;

    console::clear();
    if (scores.empty()) {
        const std::string message = "There are no available scores for this player!";
        console::move_cursor(frame_size.width / 2 - static_cast<int>(message.size()) / 2, frame_size.height / 2 - 2);
        std::cout << message;
    }
    else {
        const int name_len = static_cast<int>(name.size()) + 1;
        for (uint64_t i = 0; i < scores.size(); i++) {
            std::stringstream ss;
            ss.width(5);
            ss << (i + 1);
            ss << ". ";
            ss.width(name_len);
            ss << scores[i].name;
            ss << " -> pillars : ";
            ss.width(5);
            ss << scores[i].pillars;
            ss << " | spacebars: ";
            ss.width(5);
            ss << scores[i].spaces;
            ss << " | time: ";
            ss.width(8);
            ss.precision(2);
            ss << std::fixed;
            ss << scores[i].time;
            const std::string to_print = ss.str();

            console::move_cursor(frame_size.width / 2 - static_cast<int>(to_print.size()) / 2, static_cast<int>(2 + i));
            std::cout << to_print;
        }
    }
    std::cin.get();
}
