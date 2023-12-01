#pragma once

#include <cstddef>

struct Color
{
	using data_type = unsigned char;

	data_type r;
	data_type g;
	data_type b;
	data_type a;

	constexpr std::size_t size() noexcept { return sizeof(r) + sizeof(g) + sizeof(b) + sizeof(a); }
};

