#pragma once
#include "Probe.h"
#include <DirectXMath.h>
namespace Scene {
	class MaterialProbe: public Probe
	{
	public:
		bool VisitBuffer(class DCBuf::Buffer& material_data) override;
	protected:
		void OnSetTechnique() override;
	};
	class NodeProbe
	{
	public:
		NodeProbe() noexcept;
		bool VisitNode(class Node& node) noexcept(!IS_DEBUG);
		void SetSelectedNodeId(int node_id) noexcept;
		DirectX::XMMATRIX GetTransformMatrix() noexcept;
	private:
		int m_selected_node_id = -1;
		DirectX::XMFLOAT4X4 m_transformation;
	};
}

