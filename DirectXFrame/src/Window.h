#pragma once
#include "InitWin.h"
#include "Graphics.h"
#include "Exception.h"
#include "Keyboard.h"
#include "Mouse.h"

#include <optional>
#include <memory>
class Window
{
public:
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(
		std::wstring szWinTitle, 
		int nWidth, int nHeight,
		std::wstring szWinClass = L"DirectXFrameWork"
	);
	~Window();
	void SetWindowTitle(std::wstring szTitle);
	// c++17 required
	static std::optional<UINT> RunWindow();
	Graphics& GetGfx();
public:
	int GetWindowWidth() const;
	int GetWindowHeight() const;
public:
	Keyboard kbd;
	Mouse mouse;
private:
	static LRESULT WINAPI MsgHandlerSetUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI MsgHandlerCall(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT MsgHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	std::unique_ptr<Graphics> m_pGfx;
	HWND m_hWnd = nullptr;
	HINSTANCE m_hIns;
	std::wstring m_szWinClass;
	int m_nWidth;
	int m_nHeight;
private:
	void InitWinClass();
	void InitWindow(std::wstring szWinTitile, int nWidth, int nHeight);
};


