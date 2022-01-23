#pragma once
#include <string>
#include <Windows.h>
#include "Console.h"
class Window
{
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
	);
	~Window();
	UINT RunWindow();
	const UINT GetTerminatedParam() const;
	LRESULT MsgHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	static LRESULT WINAPI MsgHandlerSetUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI MsgHandlerCall(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
