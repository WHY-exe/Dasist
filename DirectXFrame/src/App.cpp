#include "App.h"
#include <sstream>
#include <iomanip>
App::App()
	:
	m_wnd(L"WindowTitle", 700, 500)
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
	m_wnd.GetpGfx().DrawTestTriangle(
		timer.Peek(),
		(float)m_wnd.mouse.GetMousePt().x / ((float)m_wnd.GetWindowWidth() / 2) - 1.0f,
		-((float)m_wnd.mouse.GetMousePt().y / ((float)m_wnd.GetWindowHeight() / 2) - 1.0f)
	);
	m_wnd.GetpGfx().DrawTestTriangle(
		-timer.Peek(),
		0.0f,
		0.0f
	);
	m_wnd.GetpGfx().EndFrame();
}