#include "frame.h"
#include "console.h"

#include <iostream>


frame::frame(const size& size)
{
	data.resize((size_t) size.width * size.height);
}

void frame::clear()
{
    for (auto& value : data) {
        value = ' ';
    }
}

void frame::display() const
{
    std::cout << data;
}
