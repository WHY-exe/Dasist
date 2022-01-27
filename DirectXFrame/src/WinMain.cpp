#include "Windows.h"
#include <sstream>
// standar win32 windows program
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	// 添加控制台窗口
	// ADD_CONSOLE();
	int ret = -1;
	try
	{
		Window wnd(L"DirectXFrameWork", L"Title", 500, 500);
		for (UINT msg = wnd.RunWindow(); msg != WM_QUIT; msg = wnd.RunWindow())
		{
			// mouse testing code
			while (!wnd.mouse.IsEmpty())
			{
				const auto e = wnd.mouse.ReadEvent();
				if (e.m_status == Mouse::Event::Status::Move)
				{
					std::wostringstream oss;
					POINTS mouse_pt = wnd.mouse.GetMousePt();
					oss << L"mouse pos:(" << mouse_pt.x << L"," << mouse_pt.y << L")";
					wnd.SetWindowTitle(oss.str());
				}
				if (e.m_status == Mouse::Event::Status::WheelDown)
				{
					std::wostringstream oss;
					int wheel_delta = wnd.mouse.GetWheelDelta();
					oss << L"Down:" << wheel_delta/120;
					wnd.SetWindowTitle(oss.str());
				}
				if (e.m_status == Mouse::Event::Status::WheelUp)
				{
					std::wostringstream oss;
					int wheel_delta = wnd.mouse.GetWheelDelta();
					oss << L"Up:" << wheel_delta/120;
					wnd.SetWindowTitle(oss.str());
				}
				if (e.m_status == Mouse::Event::Status::Leave)
				{
					wnd.SetWindowTitle(L"Gone");
				}
			}
		}
		ret = wnd.GetTerminatedParam();
	}
	catch (const Exception& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OKCANCEL | MB_ICONASTERISK);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standar Error", MB_OKCANCEL | MB_ICONASTERISK);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No available information", "UnKnown Error", MB_OKCANCEL | MB_ICONASTERISK);
	}
	// 关闭控制台
	// FREE_CONSOLE();
	return ret;
}

