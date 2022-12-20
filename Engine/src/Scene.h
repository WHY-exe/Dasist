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
#include "FrameCommander.h"
#include "DynamicConstantBuffer.h"
#include "ConstantBufferEx.h"
namespace Scene
{
	struct RenderOption 
	{
		std::string szModelPath;
		std::string szModelName = "Model";
		std::wstring szVSPath;
		std::wstring szPSPath;
	};
	class Mesh:public Drawable
	{
	public:
		void Submit(FrameCommander& fc, DirectX::FXMMATRIX accumulateTransform) const noexcept(!IS_DEBUG);
		CachingPixelConstantBuffer* GetMaterial() const noexcept;
		DirectX::XMMATRIX GetTransformXM() const noexcept override;
	private:
		CachingPixelConstantBuffer* m_material;
		mutable DirectX::XMFLOAT4X4 m_transform;
	};

	class Node
	{
		friend class Model;
	public:
		Node(int id, const std::wstring& NodeName, std::vector<Mesh*> pMeshes, const DirectX::XMMATRIX& transform);
		void Submit(FrameCommander& fc, DirectX::FXMMATRIX accumulateTransform) const noexcept;
		int GetId() const noexcept;
		const DirectX::XMFLOAT4X4& GetAppliedTransform() const noexcept;
		const DCBuf::Buffer* GetMaterialConstant() const noexcept;
	private:
		void AddChild(std::unique_ptr<Node> child) noexcept(!IS_DEBUG);
		void ShowTree(Node*& pSelectedNode) const noexcept(!IS_DEBUG);
		void SetAppliedTransform(DirectX::XMMATRIX transform);
		void SetAppliedMaterialConstant(const DCBuf::Buffer& buffer) const noexcept(!IS_DEBUG);
	private:
		int m_id;
		std::vector<Mesh*> m_pMeshes;
		std::vector<std::unique_ptr<Node>> m_pChilds;
		std::string m_szNodeName;
		DirectX::XMFLOAT4X4 m_BaseTransform;
		DirectX::XMFLOAT4X4 m_AppliedTransform;
	};

	class Model
	{
	private:
		class ModelWindow
		{
			friend class Model;
		public:
			void Show(const char* WindowName, const Node& node) noexcept(!IS_DEBUG);
			DirectX::XMMATRIX GetTransform() noexcept;
			std::optional<DCBuf::Buffer>& GetMaterialConstant() noexcept;
			Node* GetSelectedNode() const noexcept;
			void AppliedParameters() noexcept;
		private:
			Node* m_pSelectedNode = nullptr;
			struct TransformParams
			{
				float roll = 0.0f;
				float pitch = 0.0f;
				float yaw = 0.0f;
				float x = 0.0f;
				float y = 0.0f;
				float z = 0.0f;
				float scale = 1.0f;
			};
			struct NodeData 
			{
				bool transformDirty = false;
				TransformParams transform_data;
				bool materialCbufDirty = false;
				std::optional<DCBuf::Buffer> constant_data;
			};
			std::unordered_map<int, NodeData> m_WindowData;
		};
	public:
		Model() = default;
		Model(Graphics& gfx, RenderOption& option);
		static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, RenderOption& option, const aiMaterial* const* pMaterial);
		void Submit(FrameCommander& fc) const noexcept(!IS_DEBUG);
		std::unique_ptr<Node> ParseNode(int& next_id, const aiNode& node);
		void SpwanControlWindow() noexcept;
		void SetPos(float x, float y, float z) noexcept;
		void SetPos(DirectX::XMFLOAT3 pos) noexcept;
		void Scale(float scale) noexcept;
	private:
		std::string m_szModelName;
		std::vector<std::unique_ptr<Mesh>> m_pMeshes;
		std::unique_ptr<ModelWindow> m_pWindow;
		std::unique_ptr<Node> m_pRoot;
	};
};

