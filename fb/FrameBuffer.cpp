#include "FrameBuffer.h"
#include "Error.h"
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cassert>
#include <string.h>
//#include <chrono>

FrameBuffer::FrameBuffer(const char* path)
: m_fbDev(path),
  m_memBlock(init(2)),
  m_currentBufferId(1)
{
}

void FrameBuffer::putPixel(Point point, Color color)
{
	return putPixel(point.x, point.y, color);
}

void FrameBuffer::putPixel(int x, int y, Color color)
{
	int offset = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y + vinfo.yoffset) * finfo.line_length;
	//size_t offset = (y * m_fb_width + x) * 4;

	char* buff = buffer();

	if (vinfo.bits_per_pixel == 32) {
		buff[offset + 0] = color.b;
		buff[offset + 1] = color.g;
		buff[offset + 2] = color.r;
		buff[offset + 3] = color.a; // May not be neeeded

	} else { // assume 16bpp
		throw std::runtime_error("not supported");
	}
}

void FrameBuffer::clear(char pattern)
{
	char* buff = buffer();
	memset(buff, pattern, m_memBlock.size());
}

void FrameBuffer::enableDirectWrite(bool /*enable*/)
{
}

char* FrameBuffer::buffer()
{
	return  m_memBlock.data();
}

const char* FrameBuffer::buffer() const
{
	return m_memBlock.data();
}

void FrameBuffer::waitForVsync()
{
	int status;
	status = ioctl(m_fbDev, FBIO_WAITFORVSYNC);
	check(status);
}

MemBlock FrameBuffer::init(const int buffersCount)
{
	assert(buffersCount > 0);
	assert(buffersCount < 100);

	int result;

	memset(&finfo, 0, sizeof(struct fb_fix_screeninfo));
	memset(&vinfo, 0, sizeof(struct fb_var_screeninfo));

	result = ioctl(m_fbDev, FBIOGET_VSCREENINFO, &vinfo);
	check(result);

	vinfo.yoffset = 0;
	vinfo.yres_virtual = vinfo.yres * buffersCount;
	result = ioctl(m_fbDev, FBIOPUT_VSCREENINFO, &vinfo);
	check(result);

	result = ioctl(m_fbDev, FBIOGET_FSCREENINFO, &finfo);
	check(result);

	//m_line_length = finfo.line_length;

	//m_xoffset = vinfo.xoffset;
	//m_yoffset = vinfo.yoffset;

	void* fbdata_ptr = mmap (0, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, m_fbDev, static_cast<off_t>(0));
	if (fbdata_ptr == MAP_FAILED) {
		throw std::runtime_error("Can't mmap framebuffer bloc");
	}

	m_buffersCount = buffersCount;

	char *fbdata = static_cast<char*>(fbdata_ptr);
	return MemBlock(fbdata, finfo.smem_len);
}

void FrameBuffer::changePan(int bufId)
{
	assert(bufId < m_buffersCount);
	assert(bufId >= 0);

	int status;

	vinfo.yoffset = vinfo.yres * bufId;

	status = ioctl(m_fbDev, FBIOPAN_DISPLAY, &vinfo);
	check(status);

}

void FrameBuffer::switchToNextFramebuffer()
{
	if (m_currentBufferId < m_buffersCount)
		++m_currentBufferId;
	else
		m_currentBufferId = 0;
}

void FrameBuffer::swap()
{
	changePan(m_currentBufferId);
	switchToNextFramebuffer();

	//std::this_thread::sleep_for(std::chrono::milliseconds(20));

	waitForVsync();
}

void BresenhamLine(FrameBuffer* frameBuffer, const Point& start, const Point& end, const Color& color)
{
	assert(frameBuffer);

	int dx = abs(end.x - start.x);
	int dy = abs(end.y - start.y);

	Point curPoint = start;

	int sx = (start.x < end.x) ? 1 : -1;
	int sy = (start.y < end.y) ? 1 : -1;
	int err = dx - dy;

	while (true) {
	    frameBuffer->putPixel(curPoint, color);

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

