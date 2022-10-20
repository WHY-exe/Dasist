#pragma once
#include "InitWin.h"
#include <Windows.h>
#include <string>

std::wstring String2Utf8String(const std::string& str_in);
std::string Utf8String2String(const std::wstring& str_in);
std::wstring String2GBKString(const std::string& str_in);
std::string GBKString2String(const std::wstring& str_in);

#define ANSI_TO_UTF8_STR(input) String2Utf8String(input)
#define UTF8_TO_ANSI_STR(input) Utf8String2String(input)
#define ANSI_TO_GBK_STR(input) String2GBKString(input)
#define GBK_TO_ANSI_STR(input) GBKString2String(input)