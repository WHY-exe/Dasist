#include "Windows.h"

// standar win32 windows program
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	// 添加控制台窗口
	ADD_CONSOLE();
	int ret = -1;
	try
	{
		
		Window window(L"DirectXFrameWork", L"Title", 500, 500);
		UINT msg = window.RunWindow();
		while (msg != WM_QUIT)
		{
			msg = window.RunWindow();	
			if (window.kbd.KeyIsPressed(VK_UP))
			{
				std::cout << "Space is pressed" << std::endl;
			}		
		}
		
		ret = window.GetTerminatedParam();
	}
	catch (const Exception& e)
	{
		std::cout << e.what();
	}
	catch (const std::exception& e)
	{
		std::cout
			<< "Standar Error" << std::endl
			<< e.what();
	}
	catch (...)
	{
		std::cout
			<< "Unknow exception" << std::endl
			<< "No detail available" << std::endl;
	}
	// 关闭控制台
	FREE_CONSOLE();
	return ret;
}

