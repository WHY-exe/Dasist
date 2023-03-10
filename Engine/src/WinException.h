#pragma once
#include "InitWin.h"
#include <Windows.h>
#include <string>
#include "Exception.h"
class WinException :public Exception
{
public:
	WinException(int nLine, const char* szFile, HRESULT ErrorCode);
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept override;
protected:
	static std::string TranslateErrorCode(HRESULT ErrorCode);
	HRESULT GetErrorCode() const noexcept;
protected:
	HRESULT m_ErrorCode;
};

#ifdef _DEBUG
#define WND_EXCEPT(error_code) WinException(__LINE__, __FILE__, error_code)
#define WND_LAST_EXCEPT() WinException(__LINE__, __FILE__, GetLastError())
#define WND_CALL(hr)\
	if(FAILED(hr))\
		throw WND_EXCEPT(hr)
#else
#define WND_EXCEPT(error_code)
#define WND_LAST_EXCEPT()
#define WND_CALL(hr)\
	if(FAILED(hr))\
		throw WND_EXCEPT(hr)
#endif // !_DEBUG

