#pragma once
#include "Window.h"
#include "Timer.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include "ImguiManager.h"
class App
{
public:
	App();
	int Run();	
private:
	void DoFrame();
private:
	ImguiManager m_imguiMan;	
	Window m_wnd;
	Graphics& m_gfx;
	Timer timer;
private:
	// user data
	std::vector<std::unique_ptr<Drawable>> m_objList;
	Camera cam;
	Light light;
};

