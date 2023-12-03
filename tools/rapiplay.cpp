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

#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/manipulators/dump.hpp>

#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

namespace
{

std::vector<std::string> readPlaylist(const std::string& playlistPath)
{
	std::vector<std::string> playlist;

	BOOST_LOG_TRIVIAL(debug) << "open playlist file " << playlistPath;
	std::ifstream playlistFile(playlistPath);
	if (!playlistFile.is_open()) {
		BOOST_LOG_TRIVIAL(warning) << "can't open file " << playlistPath;
		return playlist;
	}

	std::string imagePath;
	while(playlistFile >> imagePath) {
		playlist.push_back(imagePath);
	}

	BOOST_LOG_TRIVIAL(info) << "playlist file " << playlistPath << "loaded. " << playlist.size() << " records loaded";

	return playlist;
}
} // namespace

int main(int argc, const char* argv[]) try
{
	bool frameBufferDirectWrite;
	std::string playlistFile;
	std::string frameBufferPath;

	boost::program_options::options_description genericOptions("generic");
	genericOptions.add_options()
						("help,h", "produce help message")
						("directwrite,d", boost::program_options::value<bool>(&frameBufferDirectWrite)->default_value(false), "use direct write to framebuffer")
						("playlist,p", boost::program_options::value<std::string>(&playlistFile)->required(), "playlist file")
						("framebuffer,f", boost::program_options::value<std::string>(&frameBufferPath)->default_value("/dev/fb0"), "framebuffer device")
						("verbosity,v", boost::program_options::value<std::string>(), "verbosity");

	boost::program_options::options_description cmdline_options;
	cmdline_options.add(genericOptions);

	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, cmdline_options), vm);

	if (vm.contains("help")) {
			std::cout << cmdline_options << "\n";
			return 0;
	}
	boost::program_options::notify(vm);

	// set log verbosity
	if (vm.contains("verbosity")) {
			std::cout << "verbosity" << std::endl;

			std::string verbosity = vm["verbosity"].as<std::string>();

			boost::log::trivial::severity_level severityLevel;
			boost::log::trivial::from_string(verbosity.c_str(), verbosity.size(), severityLevel);
			boost::log::core::get()->set_filter(boost::log::trivial::severity >= severityLevel);
	} else {
			boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
	}

	FrameBuffer fb(frameBufferPath.c_str());
	fb.enableDirectWrite(frameBufferDirectWrite);

	std::vector<std::string> playlist = readPlaylist(playlistFile);

	if(playlist.empty()) {
		BOOST_LOG_TRIVIAL(warning) << "no files in playlist";
		return 0;
	}

	BOOST_LOG_TRIVIAL(info) << "start main playback loop";

	size_t frame = 0;
	while(true) {
		std::ranges::for_each(playlist, [&](const auto& path) {
			BOOST_LOG_TRIVIAL(info) << "show frame " << frame;

			BOOST_LOG_TRIVIAL(debug) << "load image " << path;
			StbTexture image(path.c_str());
			if (sameDimensions(fb, image)) {
				showImage(fb, image);
			} else {
				BOOST_LOG_TRIVIAL(warning) << "resize image, performance penalty" << path;

				showImage(fb, image.resize(fb.width(), fb.height(), fb.channels()));
			}

			BOOST_LOG_TRIVIAL(debug) << "framebuffer swap";
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
