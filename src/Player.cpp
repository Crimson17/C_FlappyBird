#include "Player.h"


Player::Player() {
	this->name = "_unknown";
	this->position = 0;
	this->velocity = 0;
}

Player::Player(const std::string& name) {
	this->name = name;
	this->position = 0;
	this->velocity = 0;
}

// Updates the player physics
void Player::phys(float gravity, float deltaT) {
	this->velocity += gravity * deltaT;
	this->position += this->velocity * deltaT;
}

// Draws the player to the frame
bool Player::draw(const Frame& frame, const Size& frameSize) {
    // Getting the player frame index
    const size_t ind = size_t(this->position) * frameSize.w + frameSize.w / 4;

    // Checking if player is in the frame
    if (this->position >= 0 && this->position < frameSize.h) {
        if (frame.data[ind] != '#') {
            ((char*)&frame.data[0])[ind] = '@';
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
    return true;
}
