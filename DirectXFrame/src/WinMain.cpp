#include "App.h"
#include <sstream>
// standar win32 windows program
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	// ��ӿ���̨����
	// ADD_CONSOLE();
	try
	{
		return App{}.Run();
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
	// �رտ���̨
	// FREE_CONSOLE();
	return -1;
}

