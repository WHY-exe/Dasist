#pragma once
#include "Probe.h"
#include <DirectXMath.h>
namespace Scene {
	class MaterialProbe: public Probe
	{
	public:
		bool VisitBuffer(class DCBuf::Buffer& material_data) override;
		void SetSelectStatus(bool status) noexcept;
		bool IsSelected() const noexcept;
	protected:
		void OnSetTechnique() override;
	private:
		bool m_selected = false;
	};

	class NodeProbe
	{
	public:
		virtual bool PushNode(class Node& node) noexcept(!IS_DEBUG) = 0;
		virtual void PopNode() const noexcept(!IS_DEBUG) = 0;
	};

	class TNodeProbe : public NodeProbe
	{
	public:
		TNodeProbe() noexcept;
		bool VisitNode(class Node& node) noexcept(!IS_DEBUG);
		bool PushNode(class Node& node) noexcept(!IS_DEBUG) override;
		void PopNode() const noexcept(!IS_DEBUG) override;
		DirectX::XMMATRIX GetTransformMatrix() noexcept;
	private:
		class Node* m_pSelectedNode = nullptr;
		DirectX::XMFLOAT4X4 m_transformation;
		MaterialProbe m_matProbe;
	};
	class ModelProbe
	{
	public:
		void SpwanControlWindow(class Model& model) noexcept(!IS_DEBUG);
	private:
		TNodeProbe np;
	};
}

