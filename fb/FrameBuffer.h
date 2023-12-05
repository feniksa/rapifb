#pragma once

#include <linux/fb.h>

#include "MemBlock.h"
#include "FileDescriptor.h"
#include "Point.h"
#include "Color.h"
#include <vector>

//struct fb_fix_screeninfo;
//struct fb_var_screeninfo;

class FrameBuffer final
{
public:
	FrameBuffer(const char* path = "/dev/fb0");

	void putPixel(Point point, Color color);
	void putPixel(size_t x, size_t y, Color color);
	void line(Point start, Point end, Color color);

	void swap();

	void clear(char pattern = 0);
	int width() const noexcept { return m_fb_width; }
	int height() const noexcept { return m_fb_height; }
	int channels() const noexcept { return 4; }

	void enableDirectWrite(bool enable);

	char* buffer();
	const char* buffer() const;
private:
	MemBlock init();

	FileDescriptor m_fbDev;
	MemBlock m_memBlock;
	std::vector<char> m_backBuff;

	int m_fb_width;
	int m_fb_height;
	int m_fb_bpp;
	int m_fb_bytes;

	long int m_line_length;
	long int m_xoffset;
	long int m_yoffset;

	bool m_directWrite;

	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
};

void BresenhamLine(FrameBuffer* frameBuffer, const Point& start, const Point& end, const Color& color);
