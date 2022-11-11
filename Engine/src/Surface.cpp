#include "Surface.h"
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Surface::Surface(const std::string file_path)
	:
	m_pPixelBuffer(
		stbi_load(file_path.c_str(), &m_imgWidth, &m_imgHeight, &m_imgComp, 4),
		stbi_image_free
	),
	m_hasAlpha(false)
{
	if (m_pPixelBuffer.get() == nullptr)
	{
		using namespace std::string_literals;
		throw std::logic_error("stb_image error:检查文件格式或文件路径\n" + "文件路径："s + file_path);
	}
	unsigned int counter = 0; 
	for (const unsigned char* i = m_pPixelBuffer.get();
		i < m_pPixelBuffer.get() + (long long)m_imgWidth * m_imgHeight; 
		i++, counter++)
	{
		if (*i != 255 && (counter + 1) % 4 == 0)
		{
			m_hasAlpha = true;
			break;
		}
	}
}

const unsigned char* Surface::GetBufferPtr() const noexcept
{
	return m_pPixelBuffer.get();
}

bool Surface::HasAlpha() const noexcept
{
	return m_hasAlpha;
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
