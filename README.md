# Overview

The rapifb project is a comprehensive set of tools designed to facilitate image viewing, resizing, and playback within the Linux framebuffer environment. This suite of tools caters to the specific needs of users working with framebuffer displays, providing a seamless and efficient solution for handling images in this unique graphical environment.

1. Image Viewer for Linux Framebuffer
The image viewer tool `rapishow` allows users to seamlessly view images directly within the Linux framebuffer. With support for various image formats, users can easily navigate through their image library without the need for a full desktop environment. The tool offers a lightweight and user-friendly interface for efficient image browsing.

Exapmle of usage:
```rapishow -i someimage.jpg```

2. Image Resizer for Linux Framebuffer:
The image resizing tool `rapiresize` is a powerful utility that enables users to resize images directly for the the Linux framebuffer environment. This functionality is especially useful for optimizing images for specific framebuffer display resolutions, ensuring compatibility and optimal visual presentation. Users can customize resizing parameters to suit their preferences.

Exapmle of usage:
```rapiresize -i /tmp/original_frames -o /tmp/resized_frames```

> Supports only bmp output format now

3. Frame Player for Linux Framebuffer:
The frame player tool `rapiplay` is designed for playing sequences of frames in the Linux framebuffer. This feature is particularly beneficial for creating simple animations or viewing image sequences in a slideshow format.

Exapmle of usage:
1. Generate playlist for sequence of images
```ls -v1 *.bmp | xarg readlink -f > playlist.txt```
2. Run rapiplay
```rapiplay -p /home/feniks/playlist.txt```

> For small or battery powered devices, please, use .bmp mage format (.bmp format doesn't requried image decomparession)

## Key Features

1. Cross-Platform Compatibility
The toolkit is designed to work seamlessly across various Linux distributions, providing a consistent experience for users irrespective of their chosen environment.

2. Optimized Resource Usage:
All tools are developed with a focus on minimal resource consumption, ensuring efficient performance even in resource-constrained environments.

3. Command-Line Interface (CLI) Support:
Each tool comes equipped with a command-line interface, offering flexibility and scriptability for users who prefer to integrate these tools into their workflows or use them in automated processes.

# Project build

## Dependencies
* cmake
* gcc
* boost
* [stbimage](https://github.com/nothings/stb)

## Build project for release
1. Create build directory ```mkdir build```
2. cd into buid directory ```cd build```
3. Configure project in release mode ```cmake .. -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=/usr/local```
4. Build project ```make```
5. Install builded utils ```make install```. Installed tools will be in ```/usr/loca/bin``` directory by default

## To build project for debug
1. Create build directory ```mkdir build```
2. cd into buid directory ```cd build```
3. configure project ```cmake -G Ninja -DCMAKE_BUILD_TYPE=debug -DFORCE_COLORED_OUTPUT=true -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..```
4. compile project ```ninja```
