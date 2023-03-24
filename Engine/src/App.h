#pragma once
#include "Window.h"
#include "Timer.h"
#include "BlurOutlineRenderGraph.h"
#include "Drawable.h"
#include "CameraContainer.h"
#include "Light.h"
#include "ImguiManager.h"
#include "Scene.h"
#include "SceneProbe.h"
class App
{
public:
	App();
	WPARAM Run();	
private:
	void DoFrame();
	void DoWinLogic();
private:
	int x = 0, y = 0;
	ImguiManager m_imguiMan;	
	Window m_wnd;
	Graphics& m_gfx;
	Timer timer;
private:
	// user data
	double m_delta_time = 0.0;
	Rgph::BlurOutlineRenderGraph m_rg;
	std::vector<Scene::Model> scene;
	Scene::ModelProbe probe;
	CameraContainer cams;
	PointLight pointLight;
	GlobalLight gLight;
};

