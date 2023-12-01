#include "FrameBuffer.h"
#include "Error.h"
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cassert>
#include <string.h>

FrameBuffer::FrameBuffer(const char* path)
: m_fbDev(path),
  m_memBlock(init()),
  m_backBuff(m_memBlock.size()),
  m_directWrite(false)
{
}

void FrameBuffer::putPixel(Point point, Color color)
{
	return putPixel(point.x, point.y, color);
}

void FrameBuffer::putPixel(size_t x, size_t y, Color color)
{
	size_t offset = (x + m_xoffset) * (m_fb_bytes) + (y + m_yoffset) * m_line_length;

	//size_t offset = (y * m_fb_width + x) * 4;

	char* buff = buffer();

	if (m_fb_bpp == 32) {

	buff[offset + 0] = color.b;
	buff[offset + 1] = color.g;
	buff[offset + 2] = color.r;
	buff[offset + 3] = color.a; // May not be neeeded

	} else { // assume 16bpp
		throw std::runtime_error("not supported");
	}

}

void FrameBuffer::line(Point start, Point end, Color color)
{
	BresenhamLine(start, end, color);
}

void FrameBuffer::BresenhamLine(Point start, Point end, Color color)
{
	int dx = abs(end.x - start.x);
	int dy = abs(end.y - start.y);

	Point curPoint = start;

	int sx = (start.x < end.x) ? 1 : -1;
	int sy = (start.y < end.y) ? 1 : -1;
	int err = dx - dy;

	while (true) {
	    putPixel(curPoint, color);

	    if (curPoint.x == end.x && curPoint.y == end.y)
		break;

	    int err2 = 2 * err;

	    if (err2 > -dy) {
		err -= dy;
		curPoint.x += sx;
	    }

	    if (err2 < dx) {
		err += dx;
		curPoint.y += sy;
	    }
	}
}

void FrameBuffer::swap()
{
	//assert(m_backBuff.size() == m_memBlock.size());

	if (m_directWrite)
		return;

	memcpy(m_memBlock.data(), m_backBuff.data(), m_memBlock.size());
}

void FrameBuffer::clear(char pattern)
{
	char* buff = buffer();
	memset(buff, pattern, m_memBlock.size());
}

void FrameBuffer::enableDirectWrite(bool enable)
{
	m_directWrite = enable;
}

char* FrameBuffer::buffer()
{
	return (m_directWrite) ? m_memBlock.data() : m_backBuff.data();
}

const char* FrameBuffer::buffer() const
{
	return (m_directWrite) ? m_memBlock.data() : m_backBuff.data();
}


MemBlock FrameBuffer::init()
{
	int result;
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;


	result = ioctl(m_fbDev, FBIOGET_FSCREENINFO, &finfo);
	check(result);

	result = ioctl(m_fbDev, FBIOGET_VSCREENINFO, &vinfo);
	check(result);

	m_line_length = finfo.line_length;

	m_fb_width = vinfo.xres;
	m_fb_height = vinfo.yres;
	m_fb_bpp = vinfo.bits_per_pixel;
	m_fb_bytes = m_fb_bpp / 8;

	m_xoffset = vinfo.xoffset;
	m_yoffset = vinfo.yoffset;

	int fb_data_size = m_fb_width * m_fb_height * m_fb_bytes;

	void* fbdata_ptr = mmap (0, fb_data_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fbDev, static_cast<off_t>(0));
	if (fbdata_ptr == MAP_FAILED) {
		throw std::runtime_error("Can't mmap framebuffer bloc");
	}

	char *fbdata = static_cast<char*>(fbdata_ptr);

	return MemBlock(fbdata, fb_data_size);

}
