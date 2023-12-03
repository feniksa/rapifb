#include "fb/FrameBuffer.h"
#include "fb/StbTexture.h"
#include "fb/Utils.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/manipulators/dump.hpp>

#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

#include <iostream>
#include <exception>
#include <filesystem>

int main(int argc, const char* argv[]) try
{
	bool frameBufferDirectWrite;
	std::string inputFile;
	std::string frameBufferPath;

	boost::program_options::options_description genericOptions("generic");
    genericOptions.add_options()
                        ("help,h", "produce help message")
                        ("directwrite,d", boost::program_options::value<bool>(&frameBufferDirectWrite)->default_value(false), "use direct write to framebuffer")
                        ("input-file,i", boost::program_options::value<std::string>(&inputFile)->required(), "image file to show")
                        ("framebuffer,f", boost::program_options::value<std::string>(&frameBufferPath)->default_value("/dev/fb0"), "framebuffer device")
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

	FrameBuffer fb(frameBufferPath.c_str());
	fb.enableDirectWrite(frameBufferDirectWrite);

	StbTexture image(inputFile.c_str());
	if (sameDimensions(fb, image)) {
		showImage(fb, image);
	} else {
		showImage(fb, image.resize(fb.width(), fb.height(), fb.channels()));
	}

	fb.swap();

	return 0;
}
catch(const std::exception& e)
{
	std::cerr << e.what() << std::endl;
	return -1;
}
