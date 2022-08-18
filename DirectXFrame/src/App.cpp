#include "App.h"
#include <sstream>
#include <iomanip>
#include "Box.h"
#include "Surface.h"
#include "imgui.h"

App::App()
	:
	m_wnd(L"WindowTitle", 1000, 700),
	m_gfx(m_wnd.GetGfx()),
	light(m_gfx)
{
	for (size_t i = 0; i < 2; i++)
	{
		m_objList.push_back(std::make_unique<Box>(m_gfx, (UINT)m_objList.size() + 1));
	}
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
		DoFrame();
	}
}

void App::DoFrame()
{
	m_gfx.BeginFrame();
	//
	light.Update(m_gfx);
	light.SpwanControlWindow();
	m_gfx.SetCamera(cam.GetMatrix());
	cam.SpwanControlWindow();
	for (auto& i : m_objList)
	{
		i->Update(m_gfx);
		i->Draw(m_gfx);
		i->SpwanControlWindow();
	}
	//
	m_gfx.EndFrame();
}