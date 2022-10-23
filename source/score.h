#pragma once

#include "size.h"

#include <string>
#include <vector>


class score
{
	static std::vector<score> load(const std::string& filepath);
	
public:
	char name[20] = {};
	int spaces = 0;
	int pillars = 0;
	float time = 0.0f;

	score();
	explicit score(const std::string& name);

	void save(const std::string& filepath) const;
	
	static void list(const std::string& filepath, const size& frame_size);
	static void list(const std::string& filepath, const std::string& name, const size& frame_size);
};
