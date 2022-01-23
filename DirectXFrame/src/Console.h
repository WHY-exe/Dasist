#pragma once
#include <iostream>
#include <Windows.h>
#ifdef _DEBUG
#define ADD_CONSOLE() \
AllocConsole(); \
FILE* ConsoleStream; \
freopen_s(&ConsoleStream, "CONOUT$", "w", stdout)
#else 
#define ADD_CONSOLE()
#endif // !DEBUG


