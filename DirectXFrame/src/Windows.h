#pragma once
#include "InitWin.h"
#include <string>
#include <optional>
#include <memory>
// #include "Console.h"
#include "Graphics.h"
#include "Exception.h"
#include "Keyboard.h"
#include "Mouse.h"
class Window
{
public:
	class WindowException :public Exception
	{
	public:
		WindowException(int nLine, const char* szFile, HRESULT ErrorCode);
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
	int m_nWidth, m_nHeight;
	std::wstring m_szWinClass;
	void InitWinClass();
	void InitWindow(std::wstring szWinTitile);
public:
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(
		std::wstring szWinClass, 
		std::wstring szWinTitle, 
		int nWidth, int nHeight
	);
	~Window();
	void SetWindowTitle(std::wstring szTitle);
	// c++17 required
	static std::optional<UINT> RunWindow();
	Graphics& GetpGfx();
public:
	Keyboard kbd;
	Mouse mouse;
private:
	static LRESULT WINAPI MsgHandlerSetUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI MsgHandlerCall(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT MsgHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	std::unique_ptr<Graphics> m_pGfx;
};

#ifdef _DEBUG
#define WND_EXCEPT(error_code) Window::WindowException(__LINE__, __FILE__, error_code)
#define WND_LAST_EXCEPT() Window::WindowException(__LINE__, __FILE__, GetLastError())
#else
#define WND_EXCEPT(error_code)
#define WND_LAST_EXCEPT()
#endif // !_DEBUG


