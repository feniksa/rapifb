#include "FrameBuffer.h"
#include "StbTexture.h"
#include "Utils.h"

#include <iostream>
#include <exception>
#include <filesystem>
#include <ranges>
#include <algorithm>
#include <fstream>
#include <set>

namespace
{

std::vector<std::string> readPlaylist(const char* playlistPath)
{
	std::vector<std::string> playlist;

	std::ifstream playlistFile(playlistPath);
	if (!playlistFile.is_open()) {
		std::cerr << "can't open file " << playlistPath << "\n";
		return playlist;
	}

	std::cout << "start load images\n";

	std::string imagePath;
	while(playlistFile >> imagePath) {
		playlist.push_back(imagePath);
	}

	return playlist;
}
} // namespace

int main(int argc, const char* argv[]) try
{
	FrameBuffer fb;
	//fb.enableDirectWrite(true);

	if (argc != 2) {
		std::cerr << "Not enough args\n";
		return -2;

	}

	//fb.clear();
	
	std::cout << "load plyalist\n";
	std::vector<std::string> playlist = readPlaylist(argv[1]);

	if(playlist.empty()) {
		return 0;
	}

	std::cout << "start show images\n";
	size_t frame = 0;
	while(true) {	
		std::ranges::for_each(playlist, [&](const auto& path) {
			std::cout << "show frame " << frame << "\n";

			showImage(fb, path);
			fb.swap();

			++frame;
		});
	}


	return 0;
} 
catch(const std::exception& e)
{
	std::cerr << e.what() << std::endl;
	return -1;
}
