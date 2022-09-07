#pragma once
#include "InitWin.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include "Vertex.h"
class Scene
{

public:
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetVertexLayout();
	Scene(const std::string& szFilePath);
	const Vertex::DataBuffer& GetVBuffer() const noexcept;
	const std::vector<unsigned int>& GetIndicies() const noexcept;
	const DirectX::XMFLOAT3 NormalizeVec(const DirectX::XMFLOAT3& vec3);
private:
	Vertex::DataBuffer m_vbuf;
	std::vector<unsigned int> m_Indices;
};