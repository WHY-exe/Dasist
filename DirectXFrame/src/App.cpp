#include "App.h"
#include <sstream>
#include <iomanip>
App::App()
	:
	m_wnd(L"DirectXFrameWork", L"Title", 500, 500)
{
}

int App::Run()
{
	while (true)
	{
		// c++ 17 required
		if (const auto ecode = Window::RunWindow())
		{
			return *ecode;
		}
		DoLogic();
	}
}

void App::DoLogic()
{
	m_wnd.GetpGfx().ClearBuffer(0, 0, 1, 1);
	if (m_wnd.kbd.KeyIsPressed(VK_UP))
	{
		m_wnd.GetpGfx().ClearBuffer(1, 0, 1, 1);
	}
	m_wnd.GetpGfx().EndFrame();
}