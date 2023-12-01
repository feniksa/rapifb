#include "StbTexture.h"
#include <stdexcept>
#include <string>

#include <stb_image_resize.h>
#include <stb_image_write.h>

StbTexture::StbTexture()
: m_pixels(nullptr)
{
}

StbTexture::StbTexture(StbTexture&& other) noexcept
: m_pixels(other.m_pixels),
  m_width(other.m_width),
  m_height(other.m_height),
  m_channels(other.m_channels)
{
	other.m_pixels = nullptr;
}

StbTexture::StbTexture(int width, int height, int channels)
: m_pixels(static_cast<stbi_uc*>(malloc(width * height * channels))),
  m_width(width),
  m_height(height),
  m_channels(channels)
{
}

StbTexture::StbTexture(const char* path) 
: StbTexture()
{
	if (!load(path))
		throw std::runtime_error("Can't load file: " + std::string(path) + ". " + std::string(stbi_failure_reason()) );
}

void StbTexture::unload()
{	
	if (!m_pixels) 
		return;

	stbi_image_free(m_pixels);
	m_pixels = nullptr;

}

bool StbTexture::load(const char* path)
{
	unload();

	m_pixels = stbi_load(path, &m_width, &m_height, &m_channels, 0);
	return m_pixels != nullptr;
}

StbTexture::~StbTexture()
{
	unload();
}

Color StbTexture::getPixel(size_t x, size_t y) const
{
	size_t index = (y * m_width + x) * m_channels;

	return Color(
			m_channels >= 1 ? m_pixels[index + 0] : 0, // r
			m_channels >= 2 ? m_pixels[index + 1] : 0, // g
			m_channels >= 3 ? m_pixels[index + 2] : 0, // b
			m_channels >= 4 ? m_pixels[index + 3] : 0  // a
		   );

}

StbTexture StbTexture::resize(int width, int height) const
{
	return resize(width, height, m_channels);
}

StbTexture StbTexture::resize(int width, int height, int channels) const
{
	StbTexture texture(width, height, channels);

	if (!resize(&texture)) {
		throw std::runtime_error("Can't resize image");
	}

	return texture;
}

bool StbTexture::resize(StbTexture* out) const
{
	int result;
	result = stbir_resize_uint8_srgb(
			m_pixels, m_width, m_height, 0, 
			out->data(), out->width(), out->height(), 0, 
			out->channels(), STBIR_FLAG_ALPHA_PREMULTIPLIED, 0);
	
	return result == 1 ? true : false;
}


bool StbTexture::save_bmp(const char* filename) const
{
	int result;
	result = stbi_write_bmp(filename, m_width, m_height, m_channels, m_pixels);
	return result != 0;
}

