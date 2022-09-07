#include "Scene.h"
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <cassert>
#include "Vertex.h"
#include <stdexcept>
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif // _DEBUG


const std::vector<D3D11_INPUT_ELEMENT_DESC>& Scene::GetVertexLayout()
{
	return m_vbuf.GetLayout().GetD3DLayout();
}

Scene::Scene(const std::string& szFilePath)
	:
	m_vbuf(std::move(
		Vertex::Layout()
		.Append(Vertex::Layout::Position3D)
		.Append(Vertex::Layout::Normal)
		.Append(Vertex::Layout::Texture2D)
	))
{
	Assimp::Importer importer;
	const aiScene* m_pScene = importer.ReadFile(
			szFilePath,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
	);
	if (m_pScene == nullptr)
	{
		throw std::logic_error(importer.GetErrorString());
	}
	for(unsigned int i = 0; i < m_pScene->mNumMeshes; i++)
	{ 
		const aiMesh* pMesh = m_pScene->mMeshes[i];

		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			m_vbuf.EmplaceBack(
				NormalizeVec(*reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mVertices[i])),
				*reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mNormals[i]),
				*reinterpret_cast<DirectX::XMFLOAT2*>(&pMesh->mTextureCoords[0][i])
			);
		}
		m_Indices.reserve((size_t)pMesh->mNumFaces * 3);
		for (unsigned int f = 0; f < pMesh->mNumFaces; f++)
		{
			const aiFace& face = pMesh->mFaces[f];
			assert(face.mNumIndices == 3);
			for (unsigned int i = 0; i < 3; i++)
			{
				m_Indices.push_back(face.mIndices[i]);
			}
		}
	}
}

const Vertex::DataBuffer& Scene::GetVBuffer() const noexcept
{
	return m_vbuf;
}

const std::vector<unsigned int>& Scene::GetIndicies() const noexcept
{
	return m_Indices;
}
const DirectX::XMFLOAT3 Scene::NormalizeVec(const DirectX::XMFLOAT3& vec3)
{
	auto length = (float)sqrt(vec3.x * vec3.x + vec3.y * vec3.y + vec3.z * vec3.z);
	if (length == 0.0f)
	{
		length = 1.0f;
	}
	return { vec3.x / length, vec3.y / length, vec3.z / length };
}