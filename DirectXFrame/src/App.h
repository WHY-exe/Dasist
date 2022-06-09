#pragma once
#include "Window.h"
#include "Timer.h"
class App
{
private:
	Window m_wnd;
	void DoLogic();
	Timer timer;
public:
	App();
	int Run();
};

