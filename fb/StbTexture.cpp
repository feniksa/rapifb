#include "StbTexture.h"
#include <stdexcept>
#include <cassert>
#include <string>

#include <stb_image.h>
#include <stb_image_resize.h>
#include <stb_image_write.h>

#include <boost/log/trivial.hpp>

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
#ifndef NDEBUG
	other.m_width = 0;
	other.m_height = 0;
	other.m_pixels = 0;
#endif
}

StbTexture::StbTexture(int width, int height, int channels)
: m_pixels(static_cast<pixel_type*>(malloc(width * height * channels * sizeof(pixel_type)))),
  m_width(width),
  m_height(height),
  m_channels(channels)
{
	assert(m_pixels);
	assert(width > 0);
	assert(height > 0);
	assert(channels == 3 || channels == 4);

	assert(m_pixels != nullptr);
}

StbTexture::StbTexture(const char* path)
: StbTexture()
{
	if (!load(path))
		throw std::runtime_error("Can't load file: " + std::string(path) + ". " + std::string(stbi_failure_reason()) );
}

void StbTexture::unload() {
	if (!m_pixels)
		return;

	stbi_image_free(m_pixels);
	m_pixels = nullptr;

#ifndef NDEBUG
	m_width = 0;
	m_height = 0;
	m_channels = 0;
#endif

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

Color StbTexture::getPixel(int x, int y) const
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < m_width);
	assert(y < m_height);

	int index = (y * m_width + x) * m_channels;
	assert(index < m_width * m_height * m_channels);

	switch (m_channels) {
		case 3:
		return Color(
				m_pixels[index + 0],
				m_pixels[index + 1],
				m_pixels[index + 2],
				0);
		case 4:
		return Color(
				m_pixels[index + 0],
				m_pixels[index + 1],
				m_pixels[index + 2],
				m_pixels[index + 3]);
		default:
			throw std::runtime_error("Image has unsupported number of channels: " + std::to_string(m_channels));
	}
}

StbTexture StbTexture::resize(int width, int height) const
{
	return resize(width, height, m_channels);
}

StbTexture StbTexture::resize(int width, int height, int channels) const
{
	assert(width > 0);
	assert(height > 0);
	assert(channels > 0);

	StbTexture texture(width, height, channels);

	if (!resize(&texture)) {
		throw std::runtime_error("Can't resize image");
	}

	return texture;
}

bool StbTexture::resize(StbTexture* out) const
{
	assert(m_width > 0);
	assert(m_height > 0);
	assert(m_channels == 3 || m_channels == 4);

	assert(out);
	assert(out->width() > 0);
	assert(out->height() > 0);
	assert(out->channels() == 3 || out->channels() == 4);

	BOOST_LOG_TRIVIAL(debug) << "resize image " << m_width << "x" << m_height << ":" << m_channels
		<< " -> " << out->width() << "x" << out->height() << ":" << out->channels();

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


bool StbTexture::sameDimensions(const StbTexture& texture) const noexcept
{
	return m_width == texture.width() && m_height == texture.height() && m_channels == texture.m_channels;
}

