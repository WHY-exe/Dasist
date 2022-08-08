#pragma once
#include <string>
#include <memory>
class Surface
{
public:
	Surface(const std::string file_path);
	const unsigned char* GetBufferPtr() const noexcept;
	int GetWidth() const noexcept;
	int GetHeight() const noexcept;
	int GetComp() const noexcept;
private:
	int m_imgWidth;
	int m_imgHeight;
	int m_imgComp;
	std::unique_ptr<unsigned char[]> m_pPixelBuffer;
};

