#include "Utils.h"
#include "FrameBuffer.h"
#include "StbTexture.h"
#include <cassert>
#include <stdexcept>

void showImage(FrameBuffer& fb, const StbTexture& image)
{
	if (!sameDimensions(fb, image))
		throw std::runtime_error("framebuffer should have same size");

	for (int y = 0; y < fb.height(); ++y)
	{
		for (int x = 0; x < fb.width(); ++x)
		{
			Color pixel = image.getPixel(x, y);
			fb.putPixel(x, y, pixel);
		}
	}
}

bool sameDimensions(const FrameBuffer& fb, const StbTexture& image)
{
	return fb.width() == image.width() && fb.height() == image.height();
}

