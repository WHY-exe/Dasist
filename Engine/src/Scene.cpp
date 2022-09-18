#include "Scene.h"
#include "Topology.h"
#include "TransformCbuf.h"
#include "IndexBuffer.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "Texture.h"
#include "Sampler.h"
#include "Surface.h"
#include "ConstantBuffer.h"
#include <stdexcept>
#include "imgui.h"
#include "String2WString.h"
#pragma comment(lib, "assimp-vc142-mtd.lib")

Scene::Mesh::Mesh(Graphics& gfx, std::vector<std::unique_ptr<Bindable>>& binds)
	:
	m_szMeshName("Mesh")
{
	if (!IsStaticInitialized(m_szMeshName))
	{
		InitStaticSlot(m_szMeshName);
	}
	AddStaticBind(m_szMeshName, std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	for (auto& b : binds)
	{
		if (auto idb = dynamic_cast<IndexBuffer*>(b.get()))
		{
			AddIndexBuffer(std::unique_ptr<IndexBuffer>(idb));
			b.release();
		}
		else
		{
			AddBind(std::move(b));
		}
	}
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Scene::Mesh::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulateTransform) noexcept(!IS_DEBUG)
{
	DirectX::XMStoreFloat4x4(&m_transform, accumulateTransform);
	Drawable::Draw(gfx);
}

const std::string& Scene::Mesh::GetModelName() const noexcept
{
	return m_szMeshName;
}

DirectX::XMMATRIX Scene::Mesh::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat4x4(&m_transform);
}

Scene::Node::Node(const std::wstring& NodeName, std::vector<Mesh*> pMeshes, const DirectX::XMMATRIX& transform)
	:
	m_pMeshes(std::move(pMeshes)),
	m_wszNodeName(NodeName),
	m_szNodeName(WString2String(m_wszNodeName))
{
	DirectX::XMStoreFloat4x4(&m_transform, transform);
}

void Scene::Node::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulateTransform) const noexcept(!IS_DEBUG)
{
	for (const auto pm : m_pMeshes)
	{
		pm->Draw(gfx, accumulateTransform * DirectX::XMLoadFloat4x4(&m_transform));
	}
	for (const auto& pc : m_pChilds)
	{
		pc->Draw(gfx, accumulateTransform);
	}
}

void Scene::Node::AddChild(std::unique_ptr<Node> child) noexcept(!IS_DEBUG)
{
	assert(child);
	m_pChilds.emplace_back(std::move(child));
}

void Scene::Node::ShowTree() noexcept(!IS_DEBUG)
{
	if (ImGui::TreeNode(m_szNodeName.c_str()))
	{
		for (auto& child : m_pChilds)
		{
			child->ShowTree();
		}
		ImGui::TreePop();
	}
}

Scene::Model::Model(Graphics& gfx,const RenderOption& option)
	:
	m_pos(0.0f, 0.0f, 0.0f),
	m_rot(0.0f, 0.0f, 0.0f),
	m_szModelName(option.szModelName)
{
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(
		option.szModelPath,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices
	);
	if (pScene == nullptr)
	{
		throw std::logic_error(importer.GetErrorString());
	}

	for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
	{
		m_pMeshes.emplace_back(ParseMesh(gfx, *pScene->mMeshes[i], option));		
	}
	m_pRoot = ParseNode(*pScene->mRootNode);
}

std::unique_ptr<Scene::Mesh> Scene::Model::ParseMesh(Graphics& gfx, const aiMesh& mesh, const RenderOption& option)
{
	std::vector<std::unique_ptr<Bindable>> binds;

	Vertex::DataBuffer vtxb(
		Vertex::Layout()
		.Append(Vertex::Layout::Position3D)
		.Append(Vertex::Layout::Normal)
		.Append(Vertex::Layout::Tex2D)
	);
	for (unsigned int i = 0; i < mesh.mNumVertices; i++)
	{
		vtxb.EmplaceBack(
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mVertices[i]),
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i]),
			*reinterpret_cast<DirectX::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
		);
	}
	binds.emplace_back(std::make_unique<VertexBuffer>(gfx, vtxb));
	std::vector<UINT> indicies;
	indicies.reserve((size_t)mesh.mNumFaces * 3);
	for (unsigned int f = 0; f < mesh.mNumFaces; f++)
	{
		const aiFace& face = mesh.mFaces[f];
		assert(face.mNumIndices == 3);
		for (unsigned int i = 0; i < 3; i++)
		{
			indicies.push_back(face.mIndices[i]);
		}
	}
	binds.emplace_back(std::make_unique<IndexBuffer>(gfx, indicies));

	if (option.bHasTexture)
	{
		binds.emplace_back(std::make_unique<Texture>(gfx, Surface(option.szTexturePath)));
		binds.emplace_back(std::make_unique<Sampler>(gfx));
	}

	auto pvs = std::make_unique<VertexShader>(gfx, option.szVSPath);
	auto pvsbc = pvs->GetByteCode();
	binds.emplace_back(std::move(pvs));
	binds.emplace_back(std::make_unique<InputLayout>(gfx, vtxb.GetLayout().GetD3DLayout(), pvsbc));
	binds.emplace_back(std::make_unique<PixelShader>(gfx, option.szPSPath));
	
	struct ModelCBuffer
	{
		alignas(16) DirectX::XMFLOAT3 m_Ambient = { 0.45f, 0.45f, 0.45f };
		float specular_intesity = 0.6f;
		float specular_pow = 30.0f;
	} mcb;

	binds.emplace_back(std::make_unique<PixelConstantBuffer<ModelCBuffer>>(gfx, mcb, 1u)); 
	return std::make_unique<Mesh>(gfx, binds);
}

std::unique_ptr<Scene::Node> Scene::Model::ParseNode(const aiNode& node)
{
	const auto transform = DirectX::XMMatrixTranspose(
		DirectX::XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(&node.mTransformation))
	);
	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);
	for (UINT i = 0; i < node.mNumMeshes; i++)
	{
		UINT meshIdx = node.mMeshes[i];
		curMeshPtrs.emplace_back(m_pMeshes.at(meshIdx).get());
	}
	std::unique_ptr<Node> pNode = std::make_unique<Node>(
		String2WString(std::string(node.mName.C_Str())), 
		std::move(curMeshPtrs), transform);
	for (UINT i = 0 ; i < node.mNumChildren; i++)
	{
		pNode->AddChild(ParseNode(*node.mChildren[i]));
	}
	return pNode;
}

DirectX::XMMATRIX Scene::Model::GetTransform() const noexcept
{
	return
		DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z) *
		DirectX::XMMatrixScaling(m_Scale, m_Scale, m_Scale) *
		DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}

void Scene::Model::SpwanControlWindow() noexcept
{
	if (ImGui::Begin(m_szModelName.c_str()))
	{
		ImGui::Columns(2, nullptr, true);
		m_pRoot->ShowTree();
		ImGui::NextColumn();
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_pos.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_pos.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_pos.z, -80.0f, 80.0f, "%.1f");
		ImGui::Text("Angle");
		ImGui::SliderAngle("AngleX", &m_rot.x, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("AngleY", &m_rot.y, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("AngleZ", &m_rot.z, -180.0f, 180.0f, "%.1f");
		ImGui::Text("Scale");
		ImGui::SliderFloat("ScalFactor", &m_Scale, 0.0f, 1.0f, "%.2f");
	}
	ImGui::End();
}

void Scene::Model::Draw(Graphics& gfx) const
{
	m_pRoot->Draw(gfx, GetTransform());
}

void Scene::Model::SetPos(float x, float y, float z) noexcept
{
	m_pos = { x, y, z };
}

void Scene::Model::Scale(float scale) noexcept
{
	m_Scale = scale;
}

void Scene::Model::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	m_pos = pos;
}