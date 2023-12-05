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
	void putPixel(int x, int y, Color color);
	void line(Point start, Point end, Color color);

	void swap();

	void clear(char pattern = 0);
	int width() const noexcept { return vinfo.width; }
	int height() const noexcept { return vinfo.height; }
	int channels() const noexcept { return 4; }

	void enableDirectWrite(bool enable);

	char* buffer();
	const char* buffer() const;
private:
	MemBlock init(const int m_buffersCount);
	void waitForVsync();
	void changePan(int bufId);
	void switchToNextFramebuffer();

	FileDescriptor m_fbDev;
	MemBlock m_memBlock;

	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;

	int m_buffersCount;
	int m_currentBufferId;
};

void BresenhamLine(FrameBuffer* frameBuffer, const Point& start, const Point& end, const Color& color);
