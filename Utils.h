#pragma once

#include <filesystem>

class FrameBuffer;
class StbTexture;

void showImage(FrameBuffer& fb, const StbTexture& image);
void showImage(FrameBuffer& fb, const std::filesystem::path& path);
bool sameDimensions(const FrameBuffer& fb, const StbTexture& image);

