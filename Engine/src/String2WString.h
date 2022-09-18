#pragma once
#include "InitWin.h"
#include <Windows.h>
#include <string>
std::wstring String2WString(const std::string& str_in);
std::string WString2String(const std::wstring& str_in);