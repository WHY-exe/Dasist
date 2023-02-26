#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Texture.h"
#include "Graphics.h"
#include "Vertex.h"
#include "DynamicConstantBUffer.h"
#include <vector>
#include "IndexBuffer.h"

struct TextureInfo
{
	std::shared_ptr<Texture> m_amTex;
	std::shared_ptr<Texture> m_difTex;
	std::shared_ptr<Texture> m_specTex;
	std::shared_ptr<Texture> m_normTex;
	bool m_hasAlpha = false;
};

class MeshData
{
public:
	MeshData(Graphics& gfx, const std::string& szModelPath, const aiMesh& mesh, const aiMaterial* const* pMaterial);
	TextureInfo& GetTextures() noexcept;
	std::unique_ptr<Vertex::DataBuffer>& GetVertecies() noexcept;
	std::vector<UINT>& GetIndicies() noexcept;
	DCBuf::Buffer& GetConstData() noexcept;
	const std::wstring& GetPSPath() const noexcept;
	const std::wstring& GetVSPath() const noexcept;
	DirectX::XMFLOAT3 GetCenterPoint() const noexcept;
private:
	void SetVertecies(const aiMesh& mesh);
	void SetIndicies(const aiMesh& mesh);
	void SetMaterial(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterial);
private:
	const std::string m_szModelPath;
	std::wstring m_szPSPath;
	std::wstring m_szVSPath;

	std::unique_ptr<Vertex::DataBuffer> m_pVertexData;
	std::vector<UINT> m_indicies;
	DCBuf::Buffer m_DynamicConstData;
	unsigned int NumVertices;
	float total_x = 0, total_y = 0, total_z = 0;

	TextureInfo m_texInfo;
};