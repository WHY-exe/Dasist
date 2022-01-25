#pragma once
#include <string>
#include <Windows.h>
#include "Console.h"
#include "Exception.h"

class Window
{
public:
	class WindowsException :public Exception
	{
	public:
		WindowsException(int nLine, const char* szFile, HRESULT ErrorCode);
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT ErrorCode);
		HRESULT GetErrorCode() const noexcept;
	private:
		HRESULT m_ErrorCode;
	};
private:
	HWND m_hWnd = nullptr;
	HINSTANCE m_hIns;
	UINT m_uRetParam;
	std::wstring m_szWinClass;
	void InitWinClass();
	void InitWindow(int nWidth, int nHeight, std::wstring szWinTitile);
public:
	Window(
		std::wstring szWinClass, 
		std::wstring szWinTitle, 
		int nWidth, int nHeight
	) noexcept;
	~Window();
	UINT RunWindow();
	UINT GetTerminatedParam() const noexcept;
	LRESULT MsgHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	static LRESULT WINAPI MsgHandlerSetUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI MsgHandlerCall(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#ifdef _DEBUG
#define WND_EXCEPT(error_code) Window::Exception(__LINE__, __FILE__, error_code)
#else
#define WND_EXCEPT(error_code)
#endif // !_DEBUG


