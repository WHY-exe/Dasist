#include "App.h"
#include <sstream>
#include <iomanip>
#include "Surface.h"
#include "Signal.h"
#include "imgui.h"
#include "Vertex.h"
#include "VertexShader.h"
#include "StrManager.h"
App::App()
	:
	m_imguiMan(),
	m_wnd(L"Engine", 1000, 700),
	m_gfx(m_wnd.GetGfx()),
	pointLight(m_gfx),
	gLight(m_gfx),
	m_rg(m_gfx),
	cams(m_gfx)
{
	Scene::ModelSetting op1;
	op1.szModelPath = "res\\model\\Sponza\\sponza.obj";
	op1.szModelName = "sponza";
	scene.emplace_back(Scene::Model(m_gfx, op1));
	//Scene::RenderOption op2;
	//op2.szModelPath = "res\\model\\Lumie\\Lumie.pmx";
	//op2.szModelName = "lumie";
	//model2 = Scene::Model(m_gfx, op2);

	for (auto& i : scene)
	{
		i.LinkTechniques(m_rg);		
		i.Submit();
	}
	cams.LinkTechniques(m_rg);
	cams.Submit();
	pointLight.LinkTechniques(m_rg);
	pointLight.Submit();
}

WPARAM App::Run()
{
	while (true)
	{
		timer.Mark();
		// c++ 17 required
		if (const auto ecode = Window::RunWindow())
		{
			return *ecode;
		}
		if (m_delta_time >= (1.0f / 30.0f))
		{
			DoWinLogic();
			DoFrame();
			m_delta_time = 0.0;
		}
		m_delta_time += timer.Peek();
	}
}

void App::DoFrame()
{
	m_gfx.BeginFrame();
	SIGNAL(
		cams.signalCamAdded,
		cams.LinkAddedCamera(m_rg)
	)
	for (auto& i : scene)
	{
		i.Submit();
	}
	cams.Submit();
	pointLight.Submit();
	cams.Bind(m_gfx);

	gLight.Update(m_gfx);
	pointLight.Update(m_gfx);
    m_rg.Execute(m_gfx);

	for (auto& i : scene)
	{
		probe.SpwanControlWindow(i);
	
	}
	cams.SpawControlWindow();
	gLight.SpwanControlWindow();
	pointLight.SpwanControlWindow();
	m_rg.RenderWidgets(m_gfx);
	//
	m_gfx.EndFrame();
	m_rg.Reset();
}

void App::DoWinLogic()
{
	if (m_wnd.kbd.KeyIsPressed(VK_ESCAPE))
	{
		cams.GetCamera().ShowMouse();
	}
	if (cams.GetCamera().MouseStatus())
	{
		m_wnd.DisableCursor();
	}
	else
		m_wnd.EnableCursor();
	if (m_wnd.kbd.KeyIsPressed('W'))
	{
		cams.GetCamera().Translate(0.0f, 0.0f, 10.0f);
	}
	if (m_wnd.kbd.KeyIsPressed('A'))
	{
		cams.GetCamera().Translate(-10.0f, 0.0f, 0.0f);
	}
	if (m_wnd.kbd.KeyIsPressed('R'))
	{
		cams.GetCamera().Translate(0.0f, 10.0f, 0.0f);
	}
	if (m_wnd.kbd.KeyIsPressed('S'))
	{
		cams.GetCamera().Translate(0.0f, 0.0f, -10.0f);
	}
	if (m_wnd.kbd.KeyIsPressed('D'))
	{
		cams.GetCamera().Translate(10.0f, 0.0f, 0.0f);
	}
	if (m_wnd.kbd.KeyIsPressed('F'))
	{
		cams.GetCamera().Translate(0.0f, -10.0f, 0.0f);
	}


	while (auto d = m_wnd.mouse.ReadRawDelta())
	{
		if (!m_wnd.CursorEnabled())
		{
			cams.GetCamera().Rotate((float)d->x, (float)d->y);
		}
	}

}
