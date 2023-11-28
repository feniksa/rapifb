#pragma once

#include <stdexcept>

inline void check(int status)
{
	if (status != 0)
		throw std::runtime_error("Error");
}
