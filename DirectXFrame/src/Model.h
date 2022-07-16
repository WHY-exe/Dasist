#pragma once
#include "OBJ_Loader.h"
#include "Exception.h"
#include <sstream>
class DescException : public Exception
{
public:
	DescException(int nLine, const char* szFile, const std::string& szDesc)
		:
		Exception(nLine, szFile),
		m_szDesc(szDesc)
	{};
	const char* what() const noexcept override
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[ErrorCode]:" << GetType() << std::endl
			<< "[Description]:" << m_szDesc << std::endl
			<< GetInfoString();
		WhatInfoBuffer = oss.str();
		return WhatInfoBuffer.c_str();
	};
	virtual const char* GetType() const noexcept override
	{
		return "Desc Exception";
	};
private:
	std::string m_szDesc;
};
class Model
{
public:
	Model(const std::string& file_path)
	{
		if (!loader.LoadFile(file_path))
		{
			throw DescException(__LINE__, __FILE__, "Fail to load the model from the obj file");
		}
		for (auto i = loader.LoadedVertices.begin(); i < loader.LoadedVertices.end(); i++)
		{
			i->Normal = NormalizeVec(i->Position);
		}
	};
	
	const std::vector<objl::Vertex>& GetVertices() const
	{
		return loader.LoadedVertices;
	};
	const std::vector<unsigned int>& GetIndices() const
	{
		return loader.LoadedIndices;
	};
private:
	const objl::Vector3& NormalizeVec(const objl::Vector3& vec3)
	{
		float length = (float)sqrt(vec3.X * vec3.X + vec3.Y * vec3.Y + vec3.Z * vec3.Z);
		if (length == 0.0f)
		{
			length = 1.0f;
		}
		return vec3 / length;
	}
private:
	objl::Loader loader;
};