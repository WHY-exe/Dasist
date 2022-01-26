#include "Windows.h"
#include "resource.h"
#include <sstream>
Window::Window(std::wstring szWinClass, std::wstring szWinTitle, int nWidth, int nHeight)
	:
	m_szWinClass(szWinClass),
	m_hIns(GetModuleHandle(nullptr)),
	kbd()
{
	this->InitWinClass();
	this->InitWindow(nWidth, nHeight, szWinTitle);
}
Window::~Window()
{
	UnregisterClass(m_szWinClass.c_str(), m_hIns);
	DestroyWindow(m_hWnd);
}
UINT Window::GetTerminatedParam() const noexcept
{
	return m_uRetParam;
}
void Window::InitWinClass()
{
	kbd.OnKeyDown(20);
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MsgHandlerSetUp;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hIns;
	wcex.hIcon = static_cast<HICON>(LoadImage(
		m_hIns, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 32, 32, 0
	));
	wcex.hIconSm = static_cast<HICON>(LoadImage(
		m_hIns, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 16, 16, 0
	));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = m_szWinClass.c_str();
	
	if (!wcex.hIcon || !wcex.hIconSm)
	{
		throw WND_LAST_EXCEPT();
	}
	if (!RegisterClassEx(&wcex))
	{
		throw WND_LAST_EXCEPT();
	}
}
void Window::InitWindow(int nWidth, int nHeight, std::wstring szWinTitile)
{
	RECT rectWindow = { 0, 0, nWidth, nHeight };
	if (!AdjustWindowRect(&rectWindow, NULL, FALSE))
	{
		throw WND_LAST_EXCEPT();
	}
	this->m_hWnd = CreateWindowEx(
		NULL,
		m_szWinClass.c_str(),
		szWinTitile.c_str(),
		// window style
		WS_OVERLAPPEDWINDOW,
		// window position
		CW_USEDEFAULT, CW_USEDEFAULT,
		// window size
		rectWindow.right - rectWindow.left,
		rectWindow.bottom - rectWindow.top,
		NULL, NULL,
		m_hIns,
		this
	);
	if (this->m_hWnd == nullptr)
	{
		throw WND_LAST_EXCEPT();
	}
	// show window
	ShowWindow(this->m_hWnd, SW_NORMAL);
	UpdateWindow(this->m_hWnd);
}
UINT Window::RunWindow()
{
	MSG msg = { 0 };
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	if (msg.message == WM_QUIT)
	{
		m_uRetParam = msg.wParam;
	}
	return msg.message;
}

LRESULT WINAPI Window::MsgHandlerSetUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NCCREATE)
	{
		// 获取CreateWindow()API返回的CREATESTRUCT
		const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		// 获取实例化对象的指针(调用CreateWindow时的最后一个参数)
		Window* const pWin = static_cast<Window*>(pCreate->lpCreateParams);
		if (!pWin)
		{
			throw WND_LAST_EXCEPT();
		}
		// 将实例化的对象的指针与windows系统相关联
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWin));
		// 将原始WinProc函数由MsgHandlerSetUp替换为MsgHandlerCall
		// 在第一次调用结束后，之后的每一个消息处理都由MsgHandlerCall处理
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::MsgHandlerCall));
		return pWin->MsgHandler(hWnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT WINAPI Window::MsgHandlerCall(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 获取与windows关联的数据（实例化的对象指针）
	Window* const pWin = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// 将消息交由每一个实例对象的MsgHandler处理
	return pWin->MsgHandler(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::MsgHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		std::cout << "Window is created successfully" << std::endl;
		break;

	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	// 键盘消息
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if(!(lParam & 0x40000000) || kbd.AutoRepeatIsEnable())
		{ 
			kbd.OnKeyDown(static_cast<unsigned char>(wParam));
			std::cout << wParam << std::endl;
		}
		break;
	case WM_SYSKEYUP:
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
	case WM_KEYUP:
		kbd.OnKeyUp(static_cast<unsigned char>(wParam));
		break;
	 
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::WindowException::WindowException(int nLine, const char* szFile, HRESULT ErrorCode)
	:
	Exception(nLine, szFile),
	m_ErrorCode(ErrorCode)
{
}

const char* Window::WindowException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[ErrorCode]:" << GetErrorCode() << std::endl
		<< "[Description]:" << TranslateErrorCode(m_ErrorCode) << std::endl
		<< GetInfoString();
	WhatInfoBuffer = oss.str();
	return WhatInfoBuffer.c_str();
}

const char* Window::WindowException::GetType() const noexcept
{
	return "Windows Exception";
}

std::string Window::WindowException::TranslateErrorCode(HRESULT ErrorCode)
{
	char* pMsgBuffer = nullptr;
	DWORD nMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS ,
		nullptr, ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuffer), 0, nullptr 
	);
	if (nMsgLen == 0)
	{
		return "undefine error code";
	}
	std::string szResult = pMsgBuffer;
	LocalFree(pMsgBuffer);
	return szResult;
}

HRESULT Window::WindowException::GetErrorCode() const noexcept
{
	return m_ErrorCode;
}
