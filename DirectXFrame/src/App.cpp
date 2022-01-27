#include "App.h"

App::App()
	:
	m_wnd(L"DirectXFrameWork", L"Title", 500, 500)
{
}

int App::Run()
{
	for (UINT msg = m_wnd.RunWindow(); msg != WM_QUIT; msg = m_wnd.RunWindow())
	{
		DoLogic();
	}
	return m_wnd.GetTerminatedParam();
}

void App::DoLogic()
{

}