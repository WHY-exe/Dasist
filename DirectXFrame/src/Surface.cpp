#include "Surface.h"
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Surface::Surface(const std::string file_path)
{
	unsigned char* PixelBuffer = stbi_load(file_path.c_str(), &m_imgWidth, &m_imgHeight, &m_imgComp, 0);
	if (PixelBuffer == nullptr)
	{
		throw std::logic_error("failed to load the image file ,check the file path");
	}
	m_pPixelData = std::make_unique<Color[]>((size_t)m_imgWidth * m_imgHeight);
	switch (m_imgComp)
	{
	case 3:
	{
		for (size_t x = 0; x < (size_t)m_imgWidth; x++)
		{
			for (size_t y = 0; y < (size_t)m_imgHeight; y++)
			{
				m_pPixelData[y * m_imgWidth + x] =
					Color(
						PixelBuffer[y * m_imgWidth + x],
						PixelBuffer[y * m_imgWidth + x + 1],
						PixelBuffer[y * m_imgWidth + x + 2],
						255
					);
			}
		}
		break;
	}
	case 4:
	{	
		for (size_t x = 0; x < (size_t)m_imgWidth; x++)
		{
			for (size_t y = 0; y < (size_t)m_imgHeight; y++)
			{
				m_pPixelData[y * m_imgWidth + x] =
					Color(
						PixelBuffer[y * m_imgWidth + x],
						PixelBuffer[y * m_imgWidth + x + 1],
						PixelBuffer[y * m_imgWidth + x + 2],
						PixelBuffer[y * m_imgWidth + x + 3]
					);
			}
		}
		break;
	}
	default:
		stbi_image_free(PixelBuffer);
		throw std::logic_error("doesn't support 1 and 2 channel pixel");
		break;
	}
	stbi_image_free(PixelBuffer);
}

const Surface::Color* Surface::GetBufferPtr() const noexcept
{
	return m_pPixelData.get();
}

int Surface::GetWidth() const noexcept
{
	return m_imgWidth;
}

int Surface::GetHeight() const noexcept
{
	return m_imgHeight;
}
