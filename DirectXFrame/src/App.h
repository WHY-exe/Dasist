#pragma once
#include "Window.h"
#include "Timer.h"
#include "Box.h"
class App
{
private:
	Window m_wnd;
	void DoLogic();
	Timer timer;
	Box box;
public:
	App();
	int Run();
};

