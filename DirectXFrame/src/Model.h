#pragma once
#include "OBJ_Loader.h"
#include "InitWin.h"
#include "Exception.h"
#include <d3d11.h>
#include <sstream>

class Model
{
public:
	Model(const std::string& file_path);
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetVertexLayout() const;
	const std::vector<objl::Vertex>& GetVertices() const;
	const std::vector<unsigned int>& GetIndices() const;
private:
	const objl::Vector3& NormalizeVec(const objl::Vector3& vec3);
private:
	objl::Loader m_loader;
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> m_VertexLayout;
};

