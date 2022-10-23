#include "App.h"
#include <sstream>
#include "StrTransf.h"

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
	try
	{
		return (int)App{}.Run();
	}
	catch (const Exception& e)
	{
		const std::wstring output_msg = ANSI_TO_GBK_STR(e.what());
		const std::wstring output_type = ANSI_TO_GBK_STR(e.GetType());
		MessageBox(nullptr, output_msg.c_str(), output_type.c_str(), MB_OKCANCEL | MB_ICONASTERISK);
	}
	catch (const std::exception& e)
	{
		const std::wstring output_msg = ANSI_TO_GBK_STR(e.what());
		MessageBox(nullptr, output_msg.c_str(), L"Standard Error", MB_OKCANCEL | MB_ICONASTERISK);
	}
	catch (...)
	{
		MessageBox(nullptr, L"No available information", L"UnKnown Error", MB_OKCANCEL | MB_ICONASTERISK);
	}
	// 关闭控制台
	// FREE_CONSOLE();
	return -1;
}

