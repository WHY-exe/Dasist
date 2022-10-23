#include "Window.h"
#include "WinException.h"
#include "resource.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"
#include <sstream>
Window::Window(std::wstring szWinTitle, int nWidth, int nHeight, std::wstring szWinClass)
	:
	m_szWinClass(szWinClass),
	m_hIns(GetModuleHandle(nullptr)),
	m_nWidth(nWidth),
	m_nHeight(nHeight)
{
	this->InitWindow(szWinTitle, nWidth, nHeight);
}
Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	UnregisterClass(m_szWinClass.c_str(), m_hIns);
	DestroyWindow(m_hWnd);
}
void Window::SetWindowTitle(std::wstring szTitle)
{
	if (!SetWindowText(m_hWnd, szTitle.c_str()))
	{
		throw WND_LAST_EXCEPT();
	}
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
void Window::InitWindow(std::wstring szWinTitile, int nWidth, int nHeight)
{
	this->InitWinClass();
	RECT rectWindow = { 0, 0, nWidth, nHeight };
	if (!AdjustWindowRect(&rectWindow, WS_OVERLAPPEDWINDOW, FALSE))
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
		this // 在这个封装中，最后一个参数一定得是this！！！
	);
	if (this->m_hWnd == nullptr)
	{
		throw WND_LAST_EXCEPT();
	}
	// show window
	ShowWindow(this->m_hWnd, SW_NORMAL);
	UpdateWindow(this->m_hWnd);
	//
	ImGui_ImplWin32_Init(m_hWnd);
	// init graphics object
	m_pGfx = std::make_unique<Graphics>(m_hWnd, nWidth, nHeight);
	m_pGfx->SetProjection(
		DirectX::XMMatrixPerspectiveLH(
			1.0f, (float)m_nHeight / (float)m_nWidth,
			0.5f, 100.0f
		)
	);
	// 使用硬件raw input
	//（鼠标的移动距离直接记录而非光标的移动距离的记录）
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; // mouse page
	rid.usUsage = 0x02; // mouse page;
	rid.hwndTarget = nullptr;
	rid.dwFlags = 0;
	if (!RegisterRawInputDevices(&rid, 1u, sizeof(rid)))
	{
		throw WND_LAST_EXCEPT();
	}
}
std::optional<WPARAM> Window::RunWindow() 
{
	MSG msg = { 0 };

	while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

Graphics& Window::GetGfx()
{
	return *m_pGfx;
}

int Window::GetWindowWidth() const
{
	return m_nWidth;
}

int Window::GetWindowHeight() const
{
	return m_nHeight;
}


void Window::EnableCursor() noexcept
{
	m_bEnableCursor = true;
	ShowCursor();
	EnableImguiMouse();
	FreeCursor();
}

void Window::DisableCursor() noexcept
{
	m_bEnableCursor = false;
	HideCursor();
	DisableImguiMouse();
	ConfineCursor();
}

bool Window::CursorEnabled() const noexcept
{
	return m_bEnableCursor;
}

LRESULT WINAPI Window::MsgHandlerSetUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NCCREATE)
	{
		// 获取CreateWindow()API返回的CREATESTRUCT（即this）
		const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		// 获取实例化对象的指针(调用CreateWindow时的最后一个参数)
		Window* const pWin = static_cast<Window*>(pCreate->lpCreateParams);
		if (!pWin)
		{
			throw WND_LAST_EXCEPT();
		}
		// 将实例化的对象的指针与窗口实例相关联
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWin));
		// 将原始WinProc函数由MsgHandlerSetUp替换为MsgHandlerCall
		// 在WM_NCCREATE调用结束后，之后的每一个消息处理都由MsgHandlerCall处理
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::MsgHandlerCall));
		return pWin->MsgHandler(hWnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT WINAPI Window::MsgHandlerCall(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 获取与窗口实例关联的数据（实例化的对象指针）
	Window* const pWin = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// 将消息交由每一个实例对象的MsgHandler处理
	return pWin->MsgHandler(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::MsgHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}
	const auto& imio = ImGui::GetIO();
	switch (uMsg)
	{
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	/****************** 鼠标消息 ******************/
	case WM_LBUTTONDOWN:
	{
		mouse.OnLButtonDown(lParam);
		break;
	}	
	case WM_LBUTTONUP:
	{
		mouse.OnLButtonUp(lParam);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		mouse.OnMButtonDown(lParam);
		break;
	}
	case WM_MBUTTONUP:
	{
		mouse.OnMButtonUp(lParam);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		mouse.OnRButtonDown(lParam);
		break;
	}
	case WM_RBUTTONUP:
	{
		mouse.OnRButtonUp(lParam);
		break;
	}
	case WM_MOUSEMOVE:
	{
		POINTS pt = MAKEPOINTS(lParam);
		if (pt.x > 0 && pt.y > 0 && pt.x <= m_nWidth && pt.y <= m_nHeight)
		{
			mouse.OnMouseMove(lParam);
			if (!mouse.IsInWindow())
			{
				SetCapture(m_hWnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			// 如果鼠标左键或右键处于点击状态
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(lParam);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}

	case WM_MOUSEWHEEL:
	{
		mouse.OnMouseWheel(wParam);
		break;
	}
	/****************** 鼠标消息 ******************/

	/****************** 键盘消息 ******************/
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		// 禁用autorepeat
		if(!(lParam & 0x40000000) || kbd.AutoRepeatIsEnable())
		{ 
			kbd.OnKeyDown(static_cast<unsigned char>(wParam));
			//std::cout << wParam << std::endl;
		}
		break;

	case WM_SYSKEYUP:	
	case WM_KEYUP:
		kbd.OnKeyUp(static_cast<unsigned char>(wParam));		
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;

	/****************** 键盘消息 ******************/

	/****************** raw input 消息 ******************/
	case WM_INPUT:
	{
		UINT uRBufferSize = 0;
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			nullptr,
			&uRBufferSize,
			sizeof(RAWINPUTHEADER)
		) == -1)
		{
			break;
		}
		m_RawBuffer.resize(uRBufferSize);
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			m_RawBuffer.data(),
			&uRBufferSize,
			sizeof(RAWINPUTHEADER)
		) != uRBufferSize)
		{
			break;
		}
		auto& ri = reinterpret_cast<const RAWINPUT&>(*m_RawBuffer.data());
		if (
			ri.header.dwType == RIM_TYPEMOUSE &&
			(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0)
			)
		{
			mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
		}

		break;
	}
	/****************** raw input 消息 ******************/

	/************** 窗口大小改变消息 **************/
	case WM_SIZE:
	{
		m_nWidth = LOWORD(lParam);
		m_nHeight = HIWORD(lParam);
		if (this->m_pGfx.get() && wParam != SIZE_MINIMIZED)
		{
			m_pGfx->CleanUpRenderTarget();
			m_pGfx->ResizeFrameBuffer(m_nWidth, m_nHeight);
			m_pGfx->GetBackBufferAndCreateRenderTarget();
			m_pGfx->CreateAndSetStencilDepthView(m_nWidth, m_nHeight);
			m_pGfx->CreateAndSetViewPort(m_nWidth, m_nHeight);
			m_pGfx->SetProjection(
				DirectX::XMMatrixPerspectiveLH(
					1.0f, (float)m_nHeight / (float)m_nWidth,
					0.5f, 100.0f
				)
			);
		}
		break;
	}
	/************** 窗口大小改变消息 **************/

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Window::ShowCursor() noexcept
{
	while (::ShowCursor(TRUE) < 0);
}

void Window::HideCursor() noexcept
{
	while (::ShowCursor(FALSE) >= 0);
}

void Window::ConfineCursor() noexcept
{
	RECT crec;
	GetClientRect(m_hWnd, &crec);
	MapWindowPoints(m_hWnd, nullptr, reinterpret_cast<POINT*>(&crec), 2u);
	ClipCursor(&crec);
}

void Window::FreeCursor() noexcept
{
	ClipCursor(nullptr);
}

void Window::EnableImguiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::DisableImguiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

