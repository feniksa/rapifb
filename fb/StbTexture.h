#pragma once

#include <stb_image.h>
#include "Color.h"

class StbTexture
{
public:
	StbTexture();
	StbTexture(StbTexture&& other) noexcept;
	StbTexture(int width, int height, int channels);
	StbTexture(const char* path);
	~StbTexture();

	bool load(const char* path);
	void unload();

	bool isValid() const noexcept { return m_pixels != nullptr; }
	operator bool() const noexcept { return isValid(); }

	int width() const noexcept { return m_width; }
	int height() const noexcept { return m_height; }
	int channels() const noexcept { return m_channels; }

	unsigned char operator[](size_t index) const noexcept { return m_pixels[index]; }
	Color getPixel(size_t x, size_t y) const;

	bool resize(StbTexture* result) const;
	StbTexture resize(int width, int height) const;
	StbTexture resize(int width, int height, int channels) const;

	int getBufferSize() const noexcept { return m_width * m_height * m_channels; } 
	stbi_uc* data() { return m_pixels; }
	const stbi_uc* data() const { return m_pixels; }

	bool save_bmp(const char* filename) const;

	StbTexture(const StbTexture&)            = delete;
	StbTexture& operator=(const StbTexture&) = delete;
private:

	stbi_uc* m_pixels;
	int m_width;
	int m_height;
	int m_channels;
};
