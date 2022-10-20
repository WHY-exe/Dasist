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
	op1.szModelPath = "res\\model\\lumie.obj";
	op1.szModelName = "Lumie";
	op1.szPSPath = L"res\\cso\\TexPSSpec.cso";
	model1 = Scene::Model(m_gfx, op1);
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
		DoWinLogic();
	}
}

void App::DoFrame()
{
	m_gfx.BeginFrame();
	//
	gLight.Update(m_gfx);
	gLight.SpwanControlWindow();
	pointLight.Update(m_gfx);
	pointLight.Draw(m_gfx);
	pointLight.SpwanControlWindow();
	m_gfx.SetCamera(cam.GetMatrix());
	cam.SpwanControlWindow();
	model1.Draw(m_gfx);
	model1.SpwanControlWindow();
	//
	m_gfx.EndFrame();
}

void App::DoWinLogic()
{
	if (m_wnd.mouse.lmrButtonStatus().m_MIsPressed && m_wnd.kbd.KeyIsPressed(VK_MENU))
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
