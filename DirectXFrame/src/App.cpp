#include "App.h"
#include <sstream>
#include <iomanip>
App::App()
	:
	m_wnd(L"DirectXFrameWork", L"Title", 500, 500)
{
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
		//Window::Terminate ter = Window::RunWindow();
		//if (ter.m_bTerminated)
		//{
		//	return ter.m_uRetParam;
		//}
		DoLogic();
	}
}

void App::DoLogic()
{
	const float peek = timer.Peek();
	std::wostringstream oss;
	oss << L"Time Since Window is Created:" << std::fixed << std::setprecision(1) << peek;
	m_wnd.SetWindowTitle(oss.str());
}