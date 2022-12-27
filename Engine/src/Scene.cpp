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
#include "Blender.h"
#include "Rasterizer.h"
#include "ConstantBufferEx.h"
#include "MathTool.h"
#include <filesystem>
#ifndef NDEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif // !_Debug


CachingPixelConstantBuffer* Scene::Mesh::GetMaterial() const noexcept
{
	return m_material;
}

void Scene::Mesh::Submit(FrameCommander& fc, DirectX::FXMMATRIX accumulateTransform) const noexcept(!IS_DEBUG)
{
	DirectX::XMStoreFloat4x4(&m_transform, accumulateTransform);
	Drawable::Submit(fc);
}

DirectX::XMMATRIX Scene::Mesh::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat4x4(&m_transform);
}

int Scene::Node::GetId() const noexcept
{
	return m_id;
}

const DirectX::XMFLOAT4X4& Scene::Node::GetAppliedTransform() const noexcept
{
	return m_AppliedTransform;
}

const DCBuf::Buffer* Scene::Node::GetMaterialConstant() const noexcept
{
	if (m_pMeshes.size() == 0)
	{
		return nullptr;
	}
	return &(m_pMeshes.front()->GetMaterial()->GetBuffer());
}

void Scene::Node::Submit(FrameCommander& fc, DirectX::FXMMATRIX accumulateTransform) const noexcept
{
	const auto build =
		DirectX::XMLoadFloat4x4(&m_AppliedTransform) *
		DirectX::XMLoadFloat4x4(&m_BaseTransform) * 
		accumulateTransform;
	for (const auto pm : m_pMeshes)
	{
		pm->Submit(fc, build);
	}
	for (const auto& pc : m_pChilds)
	{
		pc->Submit(fc, build);
	}

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

void Scene::Node::SetAppliedMaterialConstant(const DCBuf::Buffer& buffer) const noexcept(!IS_DEBUG)
{
	auto pcb = m_pMeshes.front()->GetMaterial();
	assert(pcb != nullptr);
	pcb->SetBuffer(buffer);
}

void Scene::Node::AddChild(std::unique_ptr<Node> child) noexcept(!IS_DEBUG)
{
	assert(child);
	m_pChilds.emplace_back(std::move(child));
}

void Scene::Node::ShowTree(Node*& pSelectedNode) const noexcept(!IS_DEBUG)
{
	const int selected_id = (pSelectedNode == nullptr) ? -1 : pSelectedNode->GetId();
	const auto imgui_flags = ImGuiTreeNodeFlags_OpenOnArrow 
		| ((m_id == selected_id) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((m_pChilds.empty() ? ImGuiTreeNodeFlags_Leaf : 0));
	// render the node
	const auto expand = ImGui::TreeNodeEx((void*)(intptr_t)m_id, imgui_flags, m_szNodeName.c_str());
	if (ImGui::IsItemClicked() || ImGui::IsItemActivated())
	{
		pSelectedNode = const_cast<Node*>(this);
	}
	// render the child node if root expanded
	if (expand)
	{
		for (auto& child : m_pChilds)
		{
			child->ShowTree(pSelectedNode);
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
	if (ImGui::Begin(WindowName))
	{
		ImGui::Columns(2, nullptr, true);
		node.ShowTree(m_pSelectedNode);

		ImGui::NextColumn();
		auto selected_node_id = m_pSelectedNode->GetId();
		if (m_pSelectedNode)
		{
			auto i = m_WindowData.find(selected_node_id);
			if (i == m_WindowData.end())
			{
				const auto& nodeTransform = m_pSelectedNode->GetAppliedTransform();
				const auto euler_angle = math_tool::ExtraEulerAngle(nodeTransform);
				const auto translation = math_tool::ExtraTranslation(nodeTransform);
				TransformParams tp;
				tp.pitch = euler_angle.x;
				tp.yaw = euler_angle.y;
				tp.roll = euler_angle.z;
				tp.x = translation.x;
				tp.y = translation.y;
				tp.x = translation.z;
				// TODO: a node would be expected to has control of the constant buffer the mesh it manage
				// the meshes's constant buffer would be the seeting that applied to the node
				// appearently, now it expects the node has only one mesh
				// something needs to be improved later
				auto pMatConstant = m_pSelectedNode->GetMaterialConstant();
				auto buffer = pMatConstant != nullptr ? std::optional<DCBuf::Buffer>{*pMatConstant} : std::optional<DCBuf::Buffer>{};
				NodeData node_data = {};
				node_data.constant_data = std::move(buffer);
				node_data.transform_data = tp;
				std::tie(i, std::ignore) = m_WindowData.insert({ selected_node_id, std::move(node_data) });
			}
			{
				bool& dirty = i->second.transformDirty;
				auto dcheck = [&dirty](bool change) {dirty = dirty || change; };
				auto& transform = i->second.transform_data;
				ImGui::Text("Position");
				dcheck(ImGui::SliderFloat("X", &transform.x, -80.0f, 80.0f, "%.1f"));
				dcheck(ImGui::SliderFloat("Y", &transform.y, -80.0f, 80.0f, "%.1f"));
				dcheck(ImGui::SliderFloat("Z", &transform.z, -80.0f, 80.0f, "%.1f"));
				ImGui::Text("Angle");
				dcheck(ImGui::SliderAngle("AngleX", &transform.roll, -180.0f, 180.0f, "%.1f"));
				dcheck(ImGui::SliderAngle("AngleY", &transform.yaw, -180.0f, 180.0f, "%.1f"));
				dcheck(ImGui::SliderAngle("AngleZ", &transform.pitch, -180.0f, 180.0f, "%.1f"));
				ImGui::Text("Scale");
				dcheck(ImGui::SliderFloat("Scale", &transform.scale, 0.0f, 1.0f, "%.3f"));
			}
			if (i->second.constant_data)
			{
				auto& material_data = *i->second.constant_data;
				bool& dirty = i->second.materialCbufDirty;
				auto dcheck = [&dirty](bool change) {dirty = dirty || change; };
				ImGui::Text("Material");
				if (auto v = material_data["enNormal"]; v.Exists())
				{
					dcheck(ImGui::Checkbox("Normal Map", &v));
				}
			}
			ImGui::End();
		}
	}
}

void Scene::Model::ModelWindow::AppliedParameters() noexcept
{
	if (auto& trans_d = m_WindowData[m_pSelectedNode->GetId()].transformDirty)
	{
		m_pSelectedNode->SetAppliedTransform(GetTransform());
		trans_d = false;
	}
	
	if (auto& mat_d =  m_WindowData[m_pSelectedNode->GetId()].materialCbufDirty)
	{ 
		m_pSelectedNode->SetAppliedMaterialConstant(*GetMaterialConstant());
		mat_d = false;
	}
}

DirectX::XMMATRIX Scene::Model::ModelWindow::GetTransform() noexcept
{
	const auto& transform = m_WindowData[m_pSelectedNode->GetId()].transform_data;
	return
		DirectX::XMMatrixRotationRollPitchYaw(transform.roll, transform.yaw, transform.pitch) *
		DirectX::XMMatrixScaling(transform.scale, transform.scale, transform.scale) *
		DirectX::XMMatrixTranslation(transform.x, transform.y, transform.z);
}

std::optional<DCBuf::Buffer>& Scene::Model::ModelWindow::GetMaterialConstant() noexcept
{
	return m_WindowData[m_pSelectedNode->GetId()].constant_data;
}

Scene::Node* Scene::Model::ModelWindow::GetSelectedNode() const noexcept
{
	return m_pSelectedNode;
}

Scene::Model::Model(Graphics& gfx,ModelSetting& option)
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
}

std::unique_ptr<Scene::Mesh> Scene::Model::ParseMesh(Graphics& gfx, const aiMesh& mesh, ModelSetting& option, const aiMaterial* const* pMaterial)
{
	const std::filesystem::path& ModelPath = option.szModelPath;
	std::string szTexRootPath = ModelPath.parent_path().string();
	std::unique_ptr<Mesh> result = std::make_unique<Mesh>();
	Step normalDraw(0);
	Technique tech;
	Vertex::Layout vlayout = Vertex::Layout();

	if (mesh.HasPositions())
	{
		vlayout.Append(Vertex::Position3D);
	}
	if (mesh.HasNormals())
	{
		vlayout.Append(Vertex::Normal).Append(Vertex::Tangent);
	}
	if (mesh.HasTextureCoords(0))
	{
		vlayout.Append(Vertex::Tex2D);
	}
	Vertex::DataBuffer vtxb(vlayout);
	for (unsigned int i = 0; i < mesh.mNumVertices; i++)
	{
		for (size_t i_ele = 0; i_ele < vlayout.Count(); i_ele++)
		{
			switch (vlayout.ResolveByIndex(i_ele).GetType())
			{
			case Vertex::Position3D:
				vtxb.EmplaceBack(*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mVertices[i]));
				break;
			case Vertex::Normal:
				vtxb.EmplaceBack(*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i]));
				break;
			case Vertex::Tex2D:
				vtxb.EmplaceBack(*reinterpret_cast<DirectX::XMFLOAT2*>(&mesh.mTextureCoords[0][i]));
				break;
			case Vertex::Tangent:
				vtxb.EmplaceBack(*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mTangents[i]));
				break;
			case Vertex::Bitangent:
				vtxb.EmplaceBack(*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mBitangents[i]));
				break;
			default:
				assert("Bad Element Type" && false);
				break;
			}
		}
	}
	result->AddVertexBuffer(VertexBuffer::Resolve(gfx, ANSI_TO_UTF8_STR(option.szModelName + std::string(mesh.mName.C_Str())), vtxb));
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
	result->AddIndexBuffer(IndexBuffer::Resolve(gfx, ANSI_TO_UTF8_STR(option.szModelName + std::string(mesh.mName.C_Str())), indicies));

	float shininess = 30.0f;
	DirectX::XMFLOAT4 specColor = { 0.18f,0.18f,0.18f,1.0f };
	DirectX::XMFLOAT4 diffuseColor = { 0.45f,0.45f,0.85f,1.0f };
	DirectX::XMFLOAT4 ambientColor = { 0.45f, 0.45f, 0.45f, 1.0f };
	if (mesh.mMaterialIndex >= 0)
	{
		using namespace std::string_literals;
		DCBuf::RawLayout cBufferLayout;
		cBufferLayout.Add<DCBuf::Float4>("Ambient");
		cBufferLayout.Add<DCBuf::Float4>("SpecColor");
		cBufferLayout.Add<DCBuf::Float>("SpecIntensity");
		cBufferLayout.Add<DCBuf::Float>("SpecPow");
		cBufferLayout.Add<DCBuf::Bool>("hasAmbient");
		cBufferLayout.Add<DCBuf::Bool>("hasGloss");
		cBufferLayout.Add<DCBuf::Bool>("enNormal");
		DCBuf::Buffer buffer = DCBuf::Buffer(std::move(cBufferLayout));

		bool hasNormal = false, hasTex = false, hasAmbient = false, hasSpec = false, 
			hasAlpha = false, showTwoSide = false, hasAlphaGloss = false;

		std::wstring szPSPath = L"res\\cso\\PS";
		std::wstring szVSPath = L"res\\cso\\VSTex";

		auto& material = *pMaterial[mesh.mMaterialIndex];
		aiString texPath;
		if (material.GetTexture(aiTextureType_AMBIENT, 0, &texPath) == aiReturn_SUCCESS && texPath.length != 0)
		{
			auto tex = Texture::Resolve(gfx, ANSI_TO_UTF8_STR(szTexRootPath + "\\"s + texPath.C_Str()), 3);
			if (tex->HasAlpha() && !hasAlpha)
			{
				hasAlpha = true;
			}
			normalDraw.AddBind(tex);
			hasAmbient = true;
		}
		else
		{
			material.Get(AI_MATKEY_COLOR_AMBIENT, reinterpret_cast<aiColor3D&>(ambientColor));
		}
		if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == aiReturn_SUCCESS && texPath.length != 0)
		{
			auto tex = Texture::Resolve(gfx, ANSI_TO_UTF8_STR(szTexRootPath + "\\"s + texPath.C_Str()));
			if (tex->HasAlpha() && !hasAlpha)
			{
				hasAlpha = true;
			}
			normalDraw.AddBind(tex);
			hasTex = true;
		}
		else
		{
			material.Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>(diffuseColor));
		}
		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texPath) == aiReturn_SUCCESS && texPath.length != 0)
		{
			auto tex = Texture::Resolve(gfx, ANSI_TO_UTF8_STR(szTexRootPath + "\\"s + texPath.C_Str()), 1);
			hasAlphaGloss = tex->HasAlpha();
			normalDraw.AddBind(tex);
			hasSpec = true;
		}
		if(!hasAlphaGloss)
		{
			material.Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>(specColor));
		}
		if (material.GetTexture(aiTextureType_NORMALS, 0, &texPath) == aiReturn_SUCCESS && texPath.length != 0)
		{
			auto tex = Texture::Resolve(gfx, ANSI_TO_UTF8_STR(szTexRootPath + "\\"s + texPath.C_Str()), 2);
			normalDraw.AddBind(tex);
			hasNormal = true;
		}
		else {
			material.Get(AI_MATKEY_SHININESS, shininess);
		}
		szPSPath += std::wstring(hasTex ? L"Tex" : L"") + (hasSpec ? L"Spec" : L"") + (hasNormal ? L"Norm" : L"") + (hasAlpha ? L"Alpha" : L"") + L".cso";
		szVSPath += std::wstring(hasNormal ? L"Norm" : L"") + L".cso";
		showTwoSide = hasAlpha;
		normalDraw.AddBind(Blender::Resolve(gfx, hasAlpha));
		normalDraw.AddBind(Rasterizer::Resolve(gfx, showTwoSide));
		auto pvs = VertexShader::Resolve(gfx, szVSPath);
		auto pvsbc = static_cast<VertexShader&>(*pvs).GetByteCode();
		normalDraw.AddBind(std::move(pvs));
		normalDraw.AddBind(InputLayout::Resolve(gfx, vtxb.GetLayout(), pvsbc));
		normalDraw.AddBind(PixelShader::Resolve(gfx, szPSPath));

		normalDraw.AddBind(Sampler::Resolve(gfx));

		buffer["Ambient"] = ambientColor;
		buffer["SpecColor"] = specColor;
		buffer["SpecIntensity"] = 0.5f;		
		buffer["SpecPow"] = shininess;
		buffer["hasAmbient"] = hasAmbient;
		buffer["hasGloss"] = hasAlphaGloss;
		buffer["enNormal"] = true;
		normalDraw.AddBind(std::make_shared<CachingPixelConstantBuffer>(gfx, buffer, 2u));
	}
	result->AddTopology(Topology::Resolve(gfx, D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP));
	tech.AddStep(normalDraw);
	result->AddTechnique(tech);
	return result;
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
		ANSI_TO_UTF8_STR(std::string(node.mName.C_Str())), 
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

void Scene::Model::Submit(FrameCommander& fc) const noexcept(!IS_DEBUG)
{
	m_pRoot->Submit(fc, DirectX::XMMatrixIdentity());
}

void Scene::Model::SetPos(float x, float y, float z) noexcept
{
	auto& transform = m_pWindow->m_WindowData[m_pWindow->m_pSelectedNode->m_id].transform_data;
	m_pWindow->m_WindowData[m_pWindow->m_pSelectedNode->m_id].transformDirty = true;
	transform.x = x;
	transform.y = y;
	transform.z = z;
}

void Scene::Model::Scale(float scale) noexcept
{
	auto& transform = m_pWindow->m_WindowData[m_pWindow->m_pSelectedNode->m_id].transform_data;
	m_pWindow->m_WindowData[m_pWindow->m_pSelectedNode->m_id].transformDirty = true;
	transform.scale = scale;
}

void Scene::Model::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	auto& transform = m_pWindow->m_WindowData[m_pWindow->m_pSelectedNode->m_id].transform_data;
	m_pWindow->m_WindowData[m_pWindow->m_pSelectedNode->m_id].transformDirty = true;
	transform.x = pos.x;
	transform.y = pos.y;
	transform.z = pos.z;
}

