#include "Pillar.h"

#include <iostream>
#include <functional>


Pillar::Pillar() {
	this->x = 0;
	this->y = 0;
    this->speed = 20;
    this->canPass = true;
}

Pillar::Pillar(float x, float y) {
	this->x = x;
	this->y = y;
    this->speed = 20;
    this->canPass = true;
}

// Pillar phys update
int Pillar::phys(const Size& frameSize, float deltaT) {
    // Pillar movement
    this->x -= this->speed * deltaT;

    // Pillar teleportation
    if (this->x <= -4) {
        this->x = frameSize.w + std::abs(this->x) + 30;
        this->y = rand() % (frameSize.h - 15) + 5.0f;
        this->canPass = true;
    }
    // Checking if the player has passed a pillar
    else if (this->x <= ((frameSize.w / 4.0f) - 1) && canPass) {
        this->canPass = false;
        return 1;
    }
    return 0;
}

// Draws the pillar
void Pillar::draw(const Frame& frame, const Size& frameSize) {
    // Check function
    static const std::function<bool(int, int, const Size&)> InFrame = [](int x, int y, const Size& frameSize) {
        if (x >= 0 && x < frameSize.w && y >= 0 && y < frameSize.h) {
            return true;
        }
        return false;
    };

    // Drawing the upper pillar
    for (int y = 0; y < (this->y - 2); y++) {
        int xLen = int(y < (this->y - 4) ? this->x + 3 : this->x + 4);
        int xStart = int(y < (this->y - 4) ? this->x - 1 : this->x - 2);
        for (int x = xStart; x < xLen; x++) {
            if (InFrame(x, y, frameSize)) {
                ((char*)&frame.data[0])[y * frameSize.w + x] = '#';
            }
        }
    }

    // Drawing the lower pillar
    for (int y = int(this->y + 2); y < frameSize.h; y++) {
        int xLen = int(y > (this->y + 3) ? this->x + 3 : this->x + 4);
        int xStart = int(y > (this->y + 3) ? this->x - 1 : this->x - 2);
        for (int x = xStart; x < xLen; x++) {
            if (InFrame(x, y, frameSize)) {
                ((char*)&frame.data[0])[y * frameSize.w + x] = '#';
            }
        }
    }
}
