#include "pillar.h"

#include <functional>


pillar::pillar()
{}

pillar::pillar(const float x, const float y)
    : x(x), y(y)
{}

int pillar::phys(const size& frame_size, const float delta_t)
{
    x -= speed * delta_t;

    if (x <= -4) {
        x = static_cast<float>(frame_size.width) + std::abs(x) + 30;
        y = static_cast<float>(rand() % (frame_size.height - 15)) + 5.0f;
        can_pass = true;
    }
    else if (x <= (static_cast<float>(frame_size.width) / 4.0f - 1) && can_pass) {
        can_pass = false;
        return 1;
    }
    return 0;
}

void pillar::draw(const frame& frame, const size& frame_size) const
{
    static const std::function in_frame = [](const int pos_x, const int pos_y, const size& size) {
        if (pos_x >= 0 && pos_x < size.width && pos_y >= 0 && pos_y < size.height) {
            return true;
        }
        return false;
    };

    for (int pos_y = 0; pos_y < (pos_y - 2); pos_y++) {
        const int x_len = static_cast<int>(pos_y < (pos_y - 4) ? x + 3 : x + 4);
        const int x_start = static_cast<int>(pos_y < (pos_y - 4) ? x - 1 : x - 2);
        for (int pos_x = x_start; pos_x < x_len; pos_x++) {
            if (in_frame(pos_x, pos_y, frame_size)) {
                const_cast<char*>(frame.data.data())[pos_y * frame_size.width + pos_x] = '#';
            }
        }
    }

    for (int pos_y = static_cast<int>(y + 2); pos_y < frame_size.height; pos_y++) {
        const int x_len = static_cast<int>(pos_y > (pos_y + 3) ? x + 3 : x + 4);
        const int x_start = static_cast<int>(pos_y > (pos_y + 3) ? x - 1 : x - 2);
        for (int pos_x = x_start; pos_x < x_len; pos_x++) {
            if (in_frame(pos_x, pos_y, frame_size)) {
                const_cast<char*>(frame.data.data())[pos_y * frame_size.width + pos_x] = '#';
            }
        }
    }
}
