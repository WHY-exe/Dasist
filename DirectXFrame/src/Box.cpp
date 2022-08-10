#include "Box.h"
#include "Model.h"
#include "VertexBuffer.h"
#include "InputLayout.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "Topology.h"
#include "TransformCbuf.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"
#include "imgui.h"
#include <memory>
Box::Box(Graphics& gfx, UINT box_id)
	:
	m_box_id(box_id)
{
	if (!IsStaticInitialized())
	{
		Model cube("obj\\cube.obj");

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, cube.GetVertices()));

		AddStaticBind(std::make_unique<Texture>(gfx, Surface("./images/box.png")));

		AddStaticBind(std::make_unique<Sampler>(gfx));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, cube.GetIndices()));

		std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "PosNorm", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		auto pvs = std::make_unique<VertexShader>(gfx, L"cso\\TexVS.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"cso\\TexPS.cso"));

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexfromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Box::Update(float dt) noexcept
{
}

void Box::SpwanControlWindow()
{
	std::stringstream ss;
	ss << "Box" << m_box_id;
	if (ImGui::Begin(ss.str().c_str()))
	{
		ImGui::Text("Position");
		ImGui::SliderAngle("X", &m_pos_x, -1000.0f, 1000.0f, "%.1f");
		ImGui::SliderAngle("Y", &m_pos_y, -1000.0f, 1000.0f, "%.1f");
		ImGui::SliderAngle("Z", &m_pos_z, -1000.0f, 1000.0f, "%.1f");
		ImGui::Text("Angle");
		ImGui::SliderAngle("AngleX", &x_roll, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("AngleY", &y_roll, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("AngleZ", &z_roll, -180.0f, 180.0f, "%.1f");
	}
	ImGui::End();
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationZ(z_roll) *
		DirectX::XMMatrixRotationY(y_roll) *
		DirectX::XMMatrixRotationX(x_roll) *
		DirectX::XMMatrixTranslation(m_pos_x, m_pos_y, m_pos_z);
}
