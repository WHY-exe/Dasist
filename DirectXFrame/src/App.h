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
	Box box1;
	Box box2;
public:
	App();
	int Run();
};

