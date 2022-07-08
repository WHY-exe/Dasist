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
	m_wnd.GetpGfx().ClearBuffer(0.0f, 0.0f, 1.0f, 1.0f);
	m_wnd.GetpGfx().DrawTestTriangle(timer.Peek());
	m_wnd.GetpGfx().EndFrame();
}