#pragma once
#include "InitWin.h"
#include <Windows.h>
#include <string>
std::wstring String2WString(const std::string& str_in)
{
	int str_len = MultiByteToWideChar(CP_ACP, 0u, str_in.c_str(), str_in.size(), nullptr, 0);
	wchar_t* output_buffer = new wchar_t[str_len + 1u];
	MultiByteToWideChar(CP_ACP, 0u, str_in.c_str(), str_in.size(), output_buffer, str_len);
	output_buffer[str_len] = '\0';
	std::wstring wszResult = std::wstring(output_buffer);
	return wszResult;
}