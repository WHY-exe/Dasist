#pragma once
#include "Window.h"
#include "Timer.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include "ImguiManager.h"
#include "Scene.h"
class App
{
public:
	App();
	int Run();	
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
	Scene::Model model;
	Camera cam;
	PointLight pointLight;
	GlobalLight gLight;
};

