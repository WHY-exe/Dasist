#include "Windows.h"

Window::Window(std::wstring szWinClass, std::wstring szWinTitle, int nWidth, int nHeight)
	:
	m_szWinClass(szWinClass),
	m_hIns(GetModuleHandle(nullptr))
{
	this->InitWinClass();
	this->InitWindow(nWidth, nHeight, szWinTitle);
}
Window::~Window()
{
	UnregisterClass(m_szWinClass.c_str(), m_hIns);
	DestroyWindow(m_hWnd);
}
const UINT Window::GetTerminatedParam() const
{
	return m_uRetParam;
}
void Window::InitWinClass()
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MsgHandlerSetUp;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hIns;
	wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = m_szWinClass.c_str();

	RegisterClassEx(&wcex);
}
void Window::InitWindow(int nWidth, int nHeight, std::wstring szWinTitile)
{
	RECT rectWindow = { 0, 0, nWidth, nHeight };
	AdjustWindowRect(&rectWindow, NULL, FALSE);
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
		NULL
	);
	/*SetWindowLongPtr(this->m_hWnd, GWLP_WNDPROC, &Window::MsgHandler);*/
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
	case WM_CHAR:
		std::cout << static_cast<char>(wParam) << std::endl;
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
