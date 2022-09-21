#include "App.h"
#include <sstream>
#include <iomanip>
#include "Surface.h"
#include "imgui.h"
#include "Vertex.h"
#include "StrTransf.h"
App::App()
	:
	m_wnd(L"WindowTitle", 1000, 700),
	m_gfx(m_wnd.GetGfx()),
	pointLight(m_gfx),
	gLight(m_gfx)
{
	Scene::RenderOption op;
	op.szModelPath = "res\\model\\nano.gltf";
	op.szModelName = "nano suit";
	model = Scene::Model(m_gfx, op);
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
	gLight.Update(m_gfx);
	gLight.SpwanControlWindow();

	pointLight.Update(m_gfx);
	pointLight.Draw(m_gfx);
	pointLight.SpwanControlWindow();
	m_gfx.SetCamera(cam.GetMatrix());
	cam.SpwanControlWindow();
	model.Draw(m_gfx);
	model.SpwanControlWindow();
	//
	m_gfx.EndFrame();
}