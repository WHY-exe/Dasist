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
#include "ConstantBuffer.h"
namespace Scene
{
	struct ModelCBuffer
	{
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 spec_color;
		float spec_intesity = 0.6f;
		float spec_pow = 20.0f;
		BOOL enNormal = TRUE;
		BOOL hasAmbient = FALSE;
	};
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
		Mesh(Graphics& gfx, ModelCBuffer& mcb, std::vector<std::shared_ptr<Bindable>>& binds);
		void Draw(Graphics& gfx, const  ModelCBuffer& mcb, DirectX::FXMMATRIX accumulateTransform) noexcept(!IS_DEBUG);
		DirectX::XMMATRIX GetTransformXM() const noexcept override;
		void Update(Graphics& gfx, const  ModelCBuffer& mcb) noexcept;
	private:
		DirectX::XMFLOAT4X4 m_transform;
		PixelConstantBuffer<ModelCBuffer> m_PCBuffer;
	};
	class Node
	{
		friend class Model;
	public:
		Node(int id, const std::wstring& NodeName, std::vector<Mesh*> pMeshes, const DirectX::XMMATRIX& transform);
		void Draw(Graphics& gfx, const ModelCBuffer& mcb, DirectX::FXMMATRIX accumulateTransform) const noexcept(!IS_DEBUG);
	private:
		void AddChild(std::unique_ptr<Node> child) noexcept(!IS_DEBUG);
		void ShowTree(std::optional<int>& selectedIndex, Node*& pSelectedNode) const noexcept(!IS_DEBUG);
		void SetAppliedTransform(DirectX::XMMATRIX transform);
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
			void Show(const char* WindowName, const Node& node, ModelCBuffer& mcb) noexcept(!IS_DEBUG);
			DirectX::XMMATRIX GetTransform() noexcept;
			Node* GetSelectedNode() const noexcept;
		private:
			std::optional<int> m_SelectedIndex = { 0 };
			Node* m_pSelectedNode;
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
			std::unordered_map<int, TransformParams> m_transform;
		};

	public:
		Model() = default;
		Model(Graphics& gfx, RenderOption& option);
		static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, RenderOption& option, const aiMaterial* const* pMaterial, ModelCBuffer& mcb);
		std::unique_ptr<Node> ParseNode(int& next_id, const aiNode& node);
		void SpwanControlWindow() noexcept;
		void SetPos(float x, float y, float z) noexcept;
		void SetPos(DirectX::XMFLOAT3 pos) noexcept;
		void Scale(float scale) noexcept;
		void Draw(Graphics& gfx) const;
	private:
		std::string m_szModelName;
		std::vector<std::unique_ptr<Mesh>> m_pMeshes;
		std::unique_ptr<ModelWindow> m_pWindow;
		std::unique_ptr<Node> m_pRoot;
		ModelCBuffer m_CBuffer;
	};
};

