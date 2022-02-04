#include "Frame.h"

#include <iostream>

#include "Console.h"


Frame::Frame(Size size) {
	this->data.resize(size_t(size.w) * size_t(size.h));
}

// Clears the frame
void Frame::clear() {
    for (int i = 0; i < this->data.size(); i++) {
        data[i] = ' ';
    }
}

// Displays the frame
void Frame::display() {
    std::cout << data;
}
