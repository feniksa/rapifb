#include "fb/FrameBuffer.h"
#include "fb/StbTexture.h"
#include "fb/Utils.h"

#include <iostream>
#include <exception>
#include <filesystem>
#include <ranges>
#include <algorithm>
#include <fstream>
#include <set>

int main(int argc, const char* argv[]) try
{
	FrameBuffer fb;
	//fb.enableDirectWrite(true);

	if (argc != 3) {
		std::cerr << "Not enough args\n";
		return -2;

	}

	//fb.clear();

	std::filesystem::path outDir(argv[2]);
	if (!std::filesystem::is_directory(outDir)) {
		std::cerr << "output directory doesn't exist" << "\n";
		return -1;
	}

	std::cout << "start load images\n";
	std::ranges::for_each(
		std::filesystem::directory_iterator{argv[1]},
		[&](const auto& dir_entry) {
			const auto imagePath = dir_entry.path();

			if (!std::filesystem::is_regular_file(imagePath)) {
				std::cout << imagePath << " is not regular file, skip\n";
				return;
			}

			StbTexture image;
			if (!image.load(imagePath.string().c_str())) {
				std::cout << imagePath << " can't load, skip\n";
				return;
			}

			std::cout << imagePath << " loaded\n";

			if (sameDimensions(fb, image)) {
				std::cout << imagePath << " same as fb, skip\n";
				return;
			}

			StbTexture resizedImage = image.resize(fb.width(), fb.height());
			std::cout << imagePath << " resized\n";

			std::filesystem::path outFileName = outDir / imagePath.filename();
			outFileName += std::filesystem::path(".bmp");

			bool result = resizedImage.save_bmp(outFileName.string().c_str());
			if (!result) {
				throw std::runtime_error("can't save " + outFileName.string());
			}
		});

	std::cout << "finished\n";

	return 0;
}
catch(const std::exception& e)
{
	std::cerr << e.what() << std::endl;
	return -1;
}
