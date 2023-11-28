#pragma once

struct Color 
{
	using data_type = unsigned char;

	data_type r;
	data_type g;
	data_type b;
	data_type a;

	Color() = default;
	Color(data_type _r = 0, data_type _g = 0, data_type _b = 0, data_type _a = 0) : r(_r), g(_g), b(_b), a(_a) {}
};

