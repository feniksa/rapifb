#pragma once

#include <boost/noncopyable.hpp>
#include "Color.h"

class StbTexture final : private boost::noncopyable
{
public:
	using pixel_type = unsigned char;

	StbTexture();
	StbTexture(StbTexture&& other) noexcept;
	StbTexture(int width, int height, int channels);
	StbTexture(const char* path);
	~StbTexture();

	bool load(const char* path);
	void unload();

	bool isValid()  const noexcept { return m_pixels != nullptr; }
	operator bool() const noexcept { return isValid();  }

	int width()     const noexcept { return m_width;    }
	int height()    const noexcept { return m_height;   }
	int channels()  const noexcept { return m_channels; }
	int getBufferSize() const noexcept { return m_width * m_height * m_channels; }

	bool sameDimensions(const StbTexture& texture) const noexcept;

	Color getPixel(int x, int y) const;

    pixel_type*       data()       { return m_pixels; }
	const pixel_type* data() const { return m_pixels; }

	bool resize(StbTexture* result) const;
	StbTexture resize(int width, int height) const;
	StbTexture resize(int width, int height, int channels) const;

	bool save_bmp(const char* filename) const;

private:
	pixel_type* m_pixels;
	int m_width;
	int m_height;
	int m_channels;
};
