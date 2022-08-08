#include "Surface.h"
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Surface::Surface(const std::string file_path)
{
	unsigned char* pPixelBuffer = stbi_load(file_path.c_str(), &m_imgWidth, &m_imgHeight, &m_imgComp, 4);
	m_pPixelBuffer = std::make_unique<unsigned char[]>(m_imgWidth * m_imgHeight * 4);
	if (m_pPixelBuffer == nullptr)
	{
		throw std::logic_error("failed to load the image file ,check the file path");
	}
	for (size_t i = 0; i < m_imgWidth * m_imgHeight * 4; i++)
	{
		m_pPixelBuffer[i] = pPixelBuffer[i];
	}
	stbi_image_free(pPixelBuffer);
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
