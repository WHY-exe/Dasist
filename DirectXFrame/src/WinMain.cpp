#include "Windows.h"
// standar win32 windows program
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{	// ��ӿ���̨����
	ADD_CONSOLE();
	Window window(L"DirectXFrameWork", L"Title", 500, 500);
	UINT msg = window.RunWindow();
	while (msg != WM_QUIT)
	{
		msg = window.RunWindow();
	}
	// �رտ���̨
	FreeConsole();
	return window.GetTerminatedParam();
}

