#pragma once
#include "Windows.h"
class App
{
private:
	Window m_wnd;
	void DoLogic();
public:
	App();
	int Run();
};

