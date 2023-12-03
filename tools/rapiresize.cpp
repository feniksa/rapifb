#include "fb/FrameBuffer.h"
#include "fb/StbTexture.h"
#include "fb/Utils.h"

#include <iostream>
#include <exception>
#include <filesystem>
#include <ranges>
#include <algorithm>
#include <fstream>
#include <stdexcept>

#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/manipulators/dump.hpp>

#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

bool resizeImage(const std::filesystem::path& sourceImage, StbTexture* outputImage);

int main(int argc, const char* argv[]) try
{
	std::string frameBufferPath;
	std::string inputResource;
	std::string outputResource;
	int outputImageChannels;

	boost::program_options::options_description genericOptions("generic");
    genericOptions.add_options()
                        ("help", "produce help message")
                        ("inputDir,i", boost::program_options::value<std::string>(&inputResource)->required(), "input directory for images (frames) to be resized")
                        ("outputDir,o", boost::program_options::value<std::string>(&outputResource)->required(), "output directory for resized images")
                        ("framebuffer,f", boost::program_options::value<std::string>(&frameBufferPath)->default_value("/dev/fb0"), "framebuffer device to get dimensions for")
                        ("width,w", boost::program_options::value<int>(), "override destination image width")
                        ("height,h", boost::program_options::value<int>(), "override destination image width")
                        ("channels,c", boost::program_options::value<int>(&outputImageChannels)->default_value(3), "image channels (3 or 4 channels supported)")
                        ("verbosity,v", boost::program_options::value<std::string>(), "verbosity");

	boost::program_options::options_description cmdline_options;
	cmdline_options.add(genericOptions);

	boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, cmdline_options), vm);
    boost::program_options::notify(vm);

    if (vm.contains("help")) {
            std::cout << cmdline_options << "\n";
            return 0;
    }

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


	std::filesystem::path outDir(outputResource);
	if (!std::filesystem::is_directory(outDir)) {
		std::cerr << "output directory " << outDir<< " doesn't exist" << "\n";
		return -1;
	}


	BOOST_LOG_TRIVIAL(debug) << "try to open output framebuffer " << frameBufferPath;
	FrameBuffer fb(frameBufferPath.c_str());
	BOOST_LOG_TRIVIAL(debug) << "framebuffer dimmensions " << fb.width() << " " << fb.height();

	int outputImageWidth = fb.width();
	int outputImageHeight = fb.height();

	if (vm.contains("width")) {
		outputImageWidth = vm["width"].as<int>();
		BOOST_LOG_TRIVIAL(debug) << "override image width " <<  outputImageWidth;
	}
	if (vm.contains("height")) {
		outputImageHeight = vm["height"].as<int>();
		BOOST_LOG_TRIVIAL(debug) << "override image height " <<  outputImageHeight;
	}

	//outputImageChannels = vm["channels"].as<int>();
	BOOST_LOG_TRIVIAL(debug) << "image channels " <<  outputImageChannels;

	if (outputImageWidth <= 0) {
		throw std::invalid_argument("output image width should be greater then 0");
	}
	if (outputImageHeight <= 0) {
		throw std::invalid_argument("output image height should be greater then 0");
	}
	if (outputImageChannels != 3 && outputImageChannels != 4) {
		throw std::invalid_argument("output image channels should be greater then 3 or 4");
	}

	BOOST_LOG_TRIVIAL(info) << "start resize images in directory " << inputResource;

	StbTexture outputImage(outputImageWidth, outputImageHeight, outputImageChannels);

	std::ranges::for_each(
		std::filesystem::directory_iterator{inputResource},
		[&](const auto& dir_entry) {
			const std::filesystem::path imagePath = dir_entry.path();
			if (!std::filesystem::is_regular_file(imagePath)) {
				BOOST_LOG_TRIVIAL(warning) << imagePath << " is not regular file, skip";
				return;
			}

			if (!resizeImage(imagePath, &outputImage)) {
				BOOST_LOG_TRIVIAL(warning) << imagePath << " not resized, skip";
				return;
			}

			std::filesystem::path outFileName = outDir / imagePath.filename();
			outFileName += std::filesystem::path(".bmp");

			BOOST_LOG_TRIVIAL(debug) << "save image " << outFileName;
			bool result = outputImage.save_bmp(outFileName.string().c_str());
			if (!result) {
				throw std::runtime_error("can't save " + outFileName.string());
			}
		});

	BOOST_LOG_TRIVIAL(info) << "done";

	return 0;
}
catch(const std::exception& e)
{
	std::cerr << e.what() << std::endl;
	return -1;
}


bool resizeImage(const std::filesystem::path& sourceImage, StbTexture* outputImage)
{
	assert(outputImage);

	StbTexture image;
	if (!image.load(sourceImage.string().c_str())) {
		BOOST_LOG_TRIVIAL(warning) << sourceImage << " can't load, skip";
		return false;
	}

	BOOST_LOG_TRIVIAL(debug) << sourceImage << " loaded";

	if (!image.resize(outputImage)) {
		BOOST_LOG_TRIVIAL(warning) << "can't resize image " << sourceImage << ". skip";
		return false;
	}
	BOOST_LOG_TRIVIAL(info) << sourceImage << " resized";
	return true;
}
