#include "App.h"
#include <sstream>
#include <iomanip>
#include "Box.h"
#include "Surface.h"
App::App()
	:
	m_wnd(L"WindowTitle", 700, 500),
	box1(m_wnd.GetpGfx()),
	box2(m_wnd.GetpGfx())
{
	
	box1.SetPosition(1.0f, 1.0f, 4.0f);
	box2.SetPosition(0.0f, 0.0f, 3.0f);
	box1.SetRotSpeed(0.0f, 0.0f, 0.0f);
	box2.SetRotSpeed(-1.0f, 1.0f, 3.0f);
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
	m_wnd.GetpGfx().ClearBuffer(0.0f, 0.0f, 0.0f, 1.0f);
	box1.Update(timer.Peek());
	box1.Draw(m_wnd.GetpGfx());
	box2.Update(timer.Peek());
	box2.Draw(m_wnd.GetpGfx());
	m_wnd.GetpGfx().EndFrame();
}