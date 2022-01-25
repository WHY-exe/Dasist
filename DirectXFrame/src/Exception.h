#pragma once
#include <exception>
#include <string>
class Exception: public std::exception
{
private:
	int m_nLine;
	std::string m_szFile;
public:
	Exception(int nLine, const char* szFile);
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetInfoString() const noexcept;
protected:
	// in order to be modified in what function
	mutable std::string WhatInfoBuffer;

};

