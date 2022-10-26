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
#include "StrTransf.h"
#include "VS_PS_TFCB.h"
#ifndef NDEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif // !_Debug



Scene::Mesh::Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bindable>>& binds)
{
	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	for (auto& b : binds)
	{
		AddBind(std::move(b));
	}
	AddBind(std::make_shared<TransformCbuf>(gfx, *this));
}

void Scene::Mesh::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulateTransform) noexcept(!IS_DEBUG)
{
	DirectX::XMStoreFloat4x4(&m_transform, accumulateTransform);
	Drawable::Draw(gfx);
}

DirectX::XMMATRIX Scene::Mesh::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat4x4(&m_transform);
}

Scene::Node::Node(int id, const std::wstring& NodeName, std::vector<Mesh*> pMeshes, const DirectX::XMMATRIX& transform)
	:
	m_id(id),
	m_pMeshes(std::move(pMeshes)),
	m_szNodeName(UTF8_TO_ANSI_STR(NodeName))
{
	DirectX::XMStoreFloat4x4(&m_BaseTransform, transform);
	DirectX::XMStoreFloat4x4(&m_AppliedTransform, DirectX::XMMatrixIdentity());
}

void Scene::Node::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulateTransform) const noexcept(!IS_DEBUG)
{
	auto build = 
		DirectX::XMLoadFloat4x4(&m_AppliedTransform) * 
		DirectX::XMLoadFloat4x4(&m_BaseTransform) * 
		accumulateTransform;
	for (const auto pm : m_pMeshes)
	{
		pm->Draw(gfx, build);
	}
	for (const auto& pc : m_pChilds)
	{
		pc->Draw(gfx, build);
	}
}

void Scene::Node::AddChild(std::unique_ptr<Node> child) noexcept(!IS_DEBUG)
{
	assert(child);
	m_pChilds.emplace_back(std::move(child));
}

void Scene::Node::ShowTree(std::optional<int>& selectedIndex, Node*& pSelectedNode) const noexcept(!IS_DEBUG)
{
	const auto imgui_flags = ImGuiTreeNodeFlags_OpenOnArrow 
		| ((m_id == selectedIndex.value_or(-1)) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((m_pChilds.empty() ? ImGuiTreeNodeFlags_Leaf : 0));
	// render the node
	const auto expand = ImGui::TreeNodeEx((void*)(intptr_t)m_id, imgui_flags, m_szNodeName.c_str());
	if (ImGui::IsItemClicked())
	{
		selectedIndex = m_id;
		pSelectedNode = const_cast<Node*>(this);
	}
	// render the child node if root expanded
	if (expand)
	{
		for (auto& child : m_pChilds)
		{
			child->ShowTree(selectedIndex, pSelectedNode);
		}
		ImGui::TreePop();
	}
}

void Scene::Node::SetAppliedTransform(DirectX::XMMATRIX transform)
{
	DirectX::XMStoreFloat4x4(&m_AppliedTransform, transform);
}

void Scene::Model::ModelWindow::Show(const char* WindowName, const Node& node) noexcept(!IS_DEBUG)
{
	int nodeIndexTracker = 0;
	if (ImGui::Begin(WindowName))
	{
		ImGui::Columns(2, nullptr, true);
		node.ShowTree(m_SelectedIndex, m_pSelectedNode);
		ImGui::NextColumn();
		if (m_pSelectedNode != nullptr)
		{
			auto& transform = m_transform[*m_SelectedIndex];
			ImGui::Text("Position");
			ImGui::SliderFloat("X", &transform.x, -80.0f, 80.0f, "%.1f");
			ImGui::SliderFloat("Y", &transform.y, -80.0f, 80.0f, "%.1f");
			ImGui::SliderFloat("Z", &transform.z, -80.0f, 80.0f, "%.1f");
			ImGui::Text("Angle");
			ImGui::SliderAngle("AngleX", &transform.roll, -180.0f, 180.0f, "%.1f");
			ImGui::SliderAngle("AngleY", &transform.yaw, -180.0f, 180.0f, "%.1f");
			ImGui::SliderAngle("AngleZ", &transform.pitch, -180.0f, 180.0f, "%.1f");
			ImGui::Text("Scale");
			ImGui::SliderFloat("Scale", &transform.scale, 0.0f, 1.0f, "%.3f");
		}
	}
	ImGui::End();
}

DirectX::XMMATRIX Scene::Model::ModelWindow::GetTransform() noexcept
{
	const auto& transform = m_transform.at(*m_SelectedIndex);
	return
		DirectX::XMMatrixRotationRollPitchYaw(transform.roll, transform.yaw, transform.pitch) *
		DirectX::XMMatrixScaling(transform.scale, transform.scale, transform.scale) *
		DirectX::XMMatrixTranslation(transform.x, transform.y, transform.z);
}

Scene::Node* Scene::Model::ModelWindow::GetSelectedNode() const noexcept
{
	return m_pSelectedNode;
}

Scene::Model::Model(Graphics& gfx,RenderOption& option)
	:
	m_szModelName(option.szModelName),
	m_pWindow(std::make_unique<ModelWindow>())
{
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(
		option.szModelPath,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);
	if (pScene == nullptr)
	{
		throw std::logic_error(importer.GetErrorString());
	}

	for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
	{
		m_pMeshes.emplace_back(ParseMesh(gfx, *pScene->mMeshes[i], option, pScene->mMaterials));		
	}
	int next_id = 0;
	m_pRoot = ParseNode(next_id, *pScene->mRootNode);
	m_pWindow->m_pSelectedNode = const_cast<Node*>(m_pRoot.get());
	m_pWindow->m_transform.insert({ *(m_pWindow->m_SelectedIndex),  Model::ModelWindow::TransformParams{} });
}

std::unique_ptr<Scene::Mesh> Scene::Model::ParseMesh(Graphics& gfx, const aiMesh& mesh, RenderOption& option, const aiMaterial* const* pMaterial)
{
	std::vector<std::shared_ptr<Bindable>> binds;
	Vertex::DataBuffer vtxb(
		Vertex::Layout()
		.Append(Vertex::Layout::Position3D)
		.Append(Vertex::Layout::Normal)
		.Append(Vertex::Layout::Tex2D)
		.Append(Vertex::Layout::Tangent)
	);
	float shininess = 30.0f;

	if (mesh.mMaterialIndex >= 0)
	{
		bool hasNormal = false, hasTex = false, hasSpec = false;
		std::wstring szPSPath = L"res\\cso\\PS";
		std::wstring szVSPath = L"res\\cso\\VSTex";
		using namespace std::string_literals;
		auto& material = *pMaterial[mesh.mMaterialIndex];
		aiString texPath;
		if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == aiReturn_SUCCESS && texPath.length != 0)
		{
			std::string szTexPath = "./res/model/"s + texPath.C_Str();
			binds.emplace_back(Texture::Resolve(gfx, ANSI_TO_UTF8_STR(szTexPath)));
			hasTex = true;
		}
		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texPath) == aiReturn_SUCCESS && texPath.length != 0)
		{
			std::string szTexPath = "./res/model/"s + texPath.C_Str();
			binds.emplace_back(Texture::Resolve(gfx, ANSI_TO_UTF8_STR(szTexPath), 1));
			hasSpec = true;
		}
		if (material.GetTexture(aiTextureType_NORMALS, 0, &texPath) == aiReturn_SUCCESS && texPath.length != 0)
		{
			std::string szTexPath = "./res/model/"s + texPath.C_Str();
			binds.emplace_back(Texture::Resolve(gfx, ANSI_TO_UTF8_STR(szTexPath), 2));
			hasNormal = true;
		}
		else {
			material.Get(AI_MATKEY_SHININESS, shininess);
		}
		if (hasTex)
		{
			szPSPath += L"Tex";
		}
		if (hasSpec)
		{
			szPSPath += L"Spec";
		}
		if (hasNormal)
		{
			szPSPath += L"Norm";
			szVSPath += L"Norm";
		}
		szPSPath += L".cso";
		szVSPath += L".cso";
		option.szPSPath = szPSPath;
		option.szVSPath = szVSPath;
		binds.emplace_back(Sampler::Resolve(gfx));
	}

	for (unsigned int i = 0; i < mesh.mNumVertices; i++)
	{
		vtxb.EmplaceBack(
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mVertices[i]),
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i]),
			*reinterpret_cast<DirectX::XMFLOAT2*>(&mesh.mTextureCoords[0][i]),
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mTangents[i])
		);
	}
	binds.emplace_back(VertexBuffer::Resolve(gfx, ANSI_TO_UTF8_STR(option.szModelName + std::string(mesh.mName.C_Str())), vtxb));
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
	binds.emplace_back(IndexBuffer::Resolve(gfx, ANSI_TO_UTF8_STR(option.szModelName + std::string(mesh.mName.C_Str())), indicies));

	auto pvs = VertexShader::Resolve(gfx, option.szVSPath);
	auto pvsbc = static_cast<VertexShader&>(*pvs).GetByteCode();
	binds.emplace_back(std::move(pvs));
	binds.emplace_back(InputLayout::Resolve(gfx, vtxb.GetLayout(), pvsbc));
	binds.emplace_back(PixelShader::Resolve(gfx, option.szPSPath));
	
	struct ModelCBuffer
	{
		alignas(16) DirectX::XMFLOAT3 m_Ambient = { 0.45f, 0.45f, 0.45f };
		float specular_intesity = 0.6f;
		float specular_pow = 20.0f;
	} mcb;
	mcb.specular_pow = shininess;
	binds.emplace_back(PixelConstantBuffer<ModelCBuffer>::Resolve(gfx, mcb, 2u)); 
	return std::make_unique<Mesh>(gfx, binds);
}

std::unique_ptr<Scene::Node> Scene::Model::ParseNode(int& next_id, const aiNode& node)
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
		next_id,
		String2Utf8String(std::string(node.mName.C_Str())), 
		std::move(curMeshPtrs), transform);
	for (UINT i = 0 ; i < node.mNumChildren; i++)
	{
		pNode->AddChild(ParseNode(++next_id, *node.mChildren[i]));
	}
	return pNode;
}

void Scene::Model::SpwanControlWindow() noexcept
{
	m_pWindow->Show(m_szModelName.c_str(), *m_pRoot);
}

void Scene::Model::Draw(Graphics& gfx) const
{
	if (auto pNode = m_pWindow->GetSelectedNode())
	{
		pNode->SetAppliedTransform(m_pWindow->GetTransform());
	}
	m_pRoot->Draw(gfx, DirectX::XMMatrixIdentity());
}

void Scene::Model::SetPos(float x, float y, float z) noexcept
{
	auto& transform = m_pWindow->m_transform[*(m_pWindow->m_SelectedIndex)];
	transform.x = x;
	transform.y = y;
	transform.z = z;
}

void Scene::Model::Scale(float scale) noexcept
{
	auto& transform = m_pWindow->m_transform[*(m_pWindow->m_SelectedIndex)];
	transform.scale = scale;
}

void Scene::Model::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	auto& transform = m_pWindow->m_transform[*(m_pWindow->m_SelectedIndex)];
	transform.x = pos.x;
	transform.y = pos.y;
	transform.z = pos.z;
}

