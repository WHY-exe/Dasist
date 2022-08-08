#pragma once
#include <string>
#include <memory>
class Surface
{
public:
	class Color
	{
	public:
		constexpr Color() noexcept : dword() {}
		constexpr Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) noexcept
			:
			dword(a << 24u | b << 16u | g << 8u | r)
		{}
		constexpr Color(unsigned char r, unsigned char g, unsigned char b) noexcept
			:
			dword(b << 16u | g << 8u | r)
		{}
		constexpr unsigned char GetR() const noexcept
		{
			return dword & 0xFFu;
		}
		constexpr unsigned char GetG() const noexcept
		{
			return dword >> 8u & 0xFFu;
		}
		constexpr unsigned char GetB() const noexcept
		{
			return dword >> 16u & 0xFFu;
		}
		constexpr unsigned char GetA() const noexcept
		{
			return dword >> 24u & 0xFFu;
		}
		void SetR(unsigned char r) noexcept
		{
			dword = (dword & 0xFF00FFFFu) | r;
		}
		void SetG(unsigned char g) noexcept
		{
			dword = (dword & 0xFFFF00FFu) | (g << 8u);
		}
		void SetB(unsigned char b) noexcept
		{
			dword = (dword & 0xFFFFFF00u) | (b << 16u);
		}
		void SetA(unsigned char a) noexcept
		{
			dword = (dword & 0x00FFFFFFu) | (a << 24u);
		}
	private:
		unsigned int dword;
	};
public:
	Surface(const std::string file_path);
	const Color* GetBufferPtr() const noexcept;
	int GetWidth() const noexcept;
	int GetHeight() const noexcept;
private:
	int m_imgWidth;
	int m_imgHeight;
	int m_imgComp;
	std::unique_ptr<Color[]> m_pPixelData;
};

