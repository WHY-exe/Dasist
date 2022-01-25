#include "Windows.h"

// standar win32 windows program
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	int ret = 0;
	try
	{
		// 添加控制台窗口
		ADD_CONSOLE();
		Window window(L"DirectXFrameWork", L"Title", 500, 500);
		UINT msg = window.RunWindow();
		while (msg != WM_QUIT)
		{
			msg = window.RunWindow();
		}
		// 关闭控制台
		FREE_CONSOLE();
		ret = window.GetTerminatedParam();
	}
	catch (const Exception& e)
	{
		std::cout << e.what();
	}
	catch (const std::exception& e)
	{
		std::cout
			<< "Std Exception" << std::endl
			<< "[Description]:" << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "UnKnowException" << std::endl;
	}
	return ret;
}

