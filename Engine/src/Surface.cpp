#include "Surface.h"
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Surface::Surface(const std::string file_path)
	:
	m_pPixelBuffer(
		stbi_load(file_path.c_str(), &m_imgWidth, &m_imgHeight, &m_imgComp, 4),
		stbi_image_free
	)
{
	if (m_pPixelBuffer.get() == nullptr)
	{
		throw std::logic_error("failed to load the image file ,check the file path");
	}
}

const unsigned char* Surface::GetBufferPtr() const noexcept
{
	return m_pPixelBuffer.get();
}

int Surface::GetWidth() const noexcept
{
	return m_imgWidth;
}

int Surface::GetHeight() const noexcept
{
	return m_imgHeight;
}

int Surface::GetComp() const noexcept
{
	return m_imgComp;
}
