#pragma once
#include "InitWin.h"
#include <Windows.h>
#include <string>
std::wstring String2WString(const std::string& str_in)
{
	const char* c_str_in = str_in.c_str();
	int len_str_in = (int)str_in.length();
	int str_len = MultiByteToWideChar(CP_ACP, 0u, c_str_in, len_str_in, nullptr, 0);
	wchar_t* output_buffer = new wchar_t[(size_t)str_len + 1u];
	MultiByteToWideChar(CP_ACP, 0u, c_str_in, len_str_in, output_buffer, str_len);
	output_buffer[str_len] = '\0';
	std::wstring wszResult = std::wstring(output_buffer);
	return wszResult;
}