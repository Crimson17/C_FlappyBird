#include "frame.h"
#include "console.h"

#include <iostream>


frame::frame(const size& size)
{
	data.resize(static_cast<uint64_t>(size.width) * size.height);
}

void frame::clear()
{
    for (char& value : data) {
        value = ' ';
    }
}

void frame::display() const
{
    std::cout << data;
}
