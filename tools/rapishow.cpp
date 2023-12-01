#include "fb/FrameBuffer.h"
#include "fb/StbTexture.h"
#include "fb/Utils.h"

#include <iostream>
#include <exception>
#include <filesystem>

int main(int argc, const char* argv[]) try
{
	FrameBuffer fb;
	StbTexture image;
	//fb.enableDirectWrite(true);

	if (argc != 2) {
		std::cerr << "Not enough args\n";
		return -2;

	}

	if (!image.load(argv[1]))
	{
		std::cerr << "Can't load file " << argv[1] << "\n";
		return -1;
	}

	fb.clear();

	std::filesystem::path imagePath(argv[1]);

	showImage(fb, imagePath);

	fb.swap();

	return 0;
}
catch(const std::exception& e)
{
	std::cerr << e.what() << std::endl;
	return -1;
}
