#pragma once
#include <string>
#include <memory>
class Surface
{
public:
	Surface(const std::string file_path);
	const unsigned char* GetBufferPtr() const noexcept;
	bool HasAlpha() const noexcept;
	int GetWidth() const noexcept;
	int GetHeight() const noexcept;
	int GetComp() const noexcept;
private:
	int m_imgWidth;
	int m_imgHeight;
	int m_imgComp;
	bool m_hasAlpha;
	std::unique_ptr<unsigned char, void(*)(void*)> m_pPixelBuffer;
};

