#include "App.h"
#include <sstream>
#include <iomanip>
#include "Surface.h"
#include "imgui.h"
#include "Vertex.h"
#include "VertexShader.h"
#include "StrTransf.h"
App::App()
	:
	m_imguiMan(),
	m_wnd(L"WindowTitle", 1000, 700),
	m_gfx(m_wnd.GetGfx()),
	pointLight(m_gfx),
	gLight(m_gfx)
{
	Scene::RenderOption op1;
	op1.szModelPath = "res\\model\\nanosuit.obj";
	op1.szModelName = "nano";
	model1 = Scene::Model(m_gfx, op1);
	Scene::RenderOption op2;
	op2.szModelPath = "res\\model\\plain.obj";
	op2.szModelName = "brick_wall";
	model2 = Scene::Model(m_gfx, op2);
}

WPARAM App::Run()
{
	while (true)
	{
		// c++ 17 required
		if (const auto ecode = Window::RunWindow())
		{
			return *ecode;
		}
		DoFrame();
		DoWinLogic();
	}
}

void App::DoFrame()
{
	m_gfx.BeginFrame();
	//
	m_gfx.SetCamera(cam.GetMatrix());
	gLight.Update(m_gfx);
	gLight.SpwanControlWindow();
	pointLight.Update(m_gfx);
	pointLight.Draw(m_gfx);
	pointLight.SpwanControlWindow();
	
	cam.SpwanControlWindow();
	//model1.Draw(m_gfx);
	//model1.SpwanControlWindow();
	model2.Draw(m_gfx);
	model2.SpwanControlWindow();
	//
	m_gfx.EndFrame();
}

void App::DoWinLogic()
{
	if (m_wnd.kbd.KeyIsPressed(VK_ESCAPE))
	{
		cam.ShowMouse();
	}
	if (cam.MouseStatus())
	{
		m_wnd.DisableCursor();
	}
	else
		m_wnd.EnableCursor();
	if (m_wnd.kbd.KeyIsPressed('W'))
	{
		cam.Translate(0.0f, 0.0f, 0.1f);
	}
	if (m_wnd.kbd.KeyIsPressed('A'))
	{
		cam.Translate(-0.1f, 0.0f, 0.0f);
	}
	if (m_wnd.kbd.KeyIsPressed('R'))
	{
		cam.Translate(0.0f, 0.1f, 0.0f);
	}
	if (m_wnd.kbd.KeyIsPressed('S'))
	{
		cam.Translate(0.0f, 0.0f, -0.1f);
	}
	if (m_wnd.kbd.KeyIsPressed('D'))
	{
		cam.Translate(0.1f, 0.0f, 0.0f);
	}
	if (m_wnd.kbd.KeyIsPressed('F'))
	{
		cam.Translate(0.0f, -0.1f, 0.0f);
	}


	while (auto d = m_wnd.mouse.ReadRawDelta())
	{
		if (!m_wnd.CursorEnabled())
		{
			cam.Rotate((float)d->x, (float)d->y);
		}
	}

}
