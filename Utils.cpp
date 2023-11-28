#include "Utils.h"
#include "FrameBuffer.h"
#include "StbTexture.h"

void showImage(FrameBuffer& fb, const StbTexture& image) 
{
	for (int x = 0; x < fb.width(); ++x) 
	{
		for (int y = 0; y < fb.height(); ++y)
		{
			Color pixel = image.getPixel(x, y);
			fb.putPixel(x, y, pixel);
		}
	}
}

bool sameDimensions(const FrameBuffer& fb, const StbTexture& image) 
{
	return (fb.width() == image.width() && fb.height() == image.height());
}

void showImage(FrameBuffer& fb, const std::filesystem::path& path) 
{
	StbTexture image(path.string().c_str());

	if (sameDimensions(fb, image))	{
		showImage(fb, image);
	} else {
		showImage(fb, image.resize(fb.width(), fb.height()));
	}

}


