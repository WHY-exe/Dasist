#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <unordered_map>
#include "Drawable.h"
#include "SceneProbe.h"
#include "DynamicConstantBuffer.h"
#include "ConstantBufferEx.h"
#include "MeshData.h"
namespace Scene
{
	struct ModelSetting 
	{
		std::string szModelPath;
		std::string szModelName = "Model";
	};
	class Mesh:public Drawable
	{
	public:
		Mesh(Graphics& gfx, MeshData& mesh_data, const std::string& model_path) noexcept;
		void Submit(DirectX::FXMMATRIX accumulateTransform) const noexcept(!_DEBUG);
		std::string GetName() noexcept;
		DirectX::XMMATRIX GetTransformXM() const noexcept override;
		void SetTransform(DirectX::XMMATRIX transform) noexcept;
	private:
		mutable DirectX::XMFLOAT4X4 m_transform;
		std::string m_szName;
	};

	class Node
	{
		friend class Model;
	public:
		Node(int id, const std::wstring& NodeName, std::vector<Mesh*> pMeshes, const DirectX::XMMATRIX& transform);
		void Submit(DirectX::FXMMATRIX accumulateTransform) const noexcept;
		void SetSelectStatus(bool status) noexcept;
		int GetId() const noexcept;
		const std::string& GetName() const noexcept;
		bool HasChild() const noexcept;		
		bool ParentSelected() const noexcept;
		const DirectX::XMFLOAT4X4& GetAppliedTransform() const noexcept;		
		void Accept(TNodeProbe& probe) noexcept(!_DEBUG);
		void Accept(MaterialProbe& probe) noexcept(!_DEBUG);
		void AcceptToShowTree(NodeProbe& probe) noexcept(!_DEBUG);
	private:
		void AddChild(std::unique_ptr<Node> child) noexcept(!_DEBUG);
		void SetAccumulateTransform(DirectX::XMMATRIX accu_tf) noexcept(!_DEBUG);
		void SetAppliedTransform(DirectX::XMMATRIX transform);
	private:
		int m_id;
		bool m_selected = false;
		std::vector<Mesh*> m_pMeshes;
		Node* m_parent = nullptr;
		std::vector<std::unique_ptr<Node>> m_pChilds;
		std::string m_szNodeName;
		DirectX::XMFLOAT4X4 m_BaseTransform;
		DirectX::XMFLOAT4X4 m_AppliedTransform;
	};

	class Model
	{
	public:
		Model() = default;
		Model(Graphics& gfx, ModelSetting& option);
		static std::unique_ptr<Mesh> ParseMesh(
			Graphics& gfx, 
			const aiMesh& mesh, 
			ModelSetting& option, 
			const aiMaterial* const* pMaterial);
		const std::string& GetName() const noexcept;
		void Submit() const noexcept(!_DEBUG);
		void LinkTechniques(Rgph::RenderGraph& rg) noexcept(!_DEBUG);
		void ApplyTransformation() noexcept(!_DEBUG);
		std::unique_ptr<Node> ParseNode(int& next_id, const aiNode& node);
		void AcceptToShowTree(NodeProbe& probe) noexcept(!_DEBUG);
		void Accept(TNodeProbe& probe) noexcept(!_DEBUG);
		void SetPos(float x, float y, float z) noexcept;
		void SetPos(DirectX::XMFLOAT3 pos) noexcept;
		void Scale(float scale) noexcept;
	private:
		TNodeProbe node_probe;
		std::string m_szModelName;
		std::vector<std::unique_ptr<Mesh>> m_pMeshes;
		std::unique_ptr<Node> m_pRoot;
		DirectX::XMFLOAT3 m_pos = {0.0f, 0.0f, 0.0f};
		float m_scale = 1.0f;
	};
};

