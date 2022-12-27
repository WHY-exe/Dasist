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
	Scene::ModelSetting op1;
	op1.szModelPath = "res\\model\\Sponza\\sponza.obj";
	op1.szModelName = "sponza";
	model1 = Scene::Model(m_gfx, op1);
	model1.Submit(fc);
	//Scene::RenderOption op2;
	//op2.szModelPath = "res\\model\\Lumie\\Lumie.pmx";
	//op2.szModelName = "lumie";
	//model2 = Scene::Model(m_gfx, op2);
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
		DoWinLogic();
		DoFrame();
	}
}

void App::DoFrame()
{
	m_gfx.BeginFrame();
	//
	cam.SpwanControlWindow();
	m_gfx.SetCamera(cam.GetMatrix());
	gLight.Update(m_gfx, cam.GetMatrix());
	pointLight.Update(m_gfx, cam.GetMatrix());
	fc.Excecute(m_gfx);
	gLight.SpwanControlWindow();
	pointLight.SpwanControlWindow();
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
		cam.Translate(0.0f, 0.0f, 10.0f);
	}
	if (m_wnd.kbd.KeyIsPressed('A'))
	{
		cam.Translate(-10.0f, 0.0f, 0.0f);
	}
	if (m_wnd.kbd.KeyIsPressed('R'))
	{
		cam.Translate(0.0f, 10.0f, 0.0f);
	}
	if (m_wnd.kbd.KeyIsPressed('S'))
	{
		cam.Translate(0.0f, 0.0f, -10.0f);
	}
	if (m_wnd.kbd.KeyIsPressed('D'))
	{
		cam.Translate(10.0f, 0.0f, 0.0f);
	}
	if (m_wnd.kbd.KeyIsPressed('F'))
	{
		cam.Translate(0.0f, -10.0f, 0.0f);
	}


	while (auto d = m_wnd.mouse.ReadRawDelta())
	{
		if (!m_wnd.CursorEnabled())
		{
			cam.Rotate((float)d->x, (float)d->y);
		}
	}

}
