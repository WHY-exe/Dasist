#include "App.h"
#include <sstream>
#include <iomanip>
#include "ModelObj.h"
#include "Surface.h"
#include "imgui.h"
#include "Scene.h"
#include "Vertex.h"
void f()
{
	Vertex::DataBuffer vdb(std::move(
		Vertex::Layout()
		.Append(Vertex::Layout::Position3D)
		.Append(Vertex::Layout::Normal)
	));
	vdb.EmplaceBack(DirectX::XMFLOAT3{ 1.0f,1.0f,5.0f }, DirectX::XMFLOAT3{ 2.0f,1.0f,4.0f });
	auto pos = vdb[0].Attri<Vertex::Layout::Position3D>();
	const auto& cpos = vdb[0].Attri<Vertex::Layout::Normal>();
}
App::App()
	:
	m_wnd(L"WindowTitle", 1000, 700),
	m_gfx(m_wnd.GetGfx()),
	pointLight(m_gfx),
	gLight(m_gfx)
{
	f();
	m_objList.push_back(std::make_unique<ModelObj>(m_gfx, "res\\model\\sphere.obj", "sphere"));
	for (size_t i = 0; i < 2; i++)
	{
		m_objList.push_back(std::make_unique<ModelObj>(m_gfx, Surface("./res/images/box.png"), "res\\model\\cube.obj","box"));
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
	gLight.Update(m_gfx);
	gLight.SpwanControlWindow();

	pointLight.Update(m_gfx);
	pointLight.Draw(m_gfx);
	pointLight.SpwanControlWindow();
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