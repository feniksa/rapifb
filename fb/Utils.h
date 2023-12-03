#pragma once

class FrameBuffer;
class StbTexture;

void showImage(FrameBuffer& fb, const StbTexture& image);

[[nodiscard]]
bool sameDimensions(const FrameBuffer& fb, const StbTexture& image);
