#include "Box.h"
#include "Model.h"
#include "VertexBuffer.h"
#include "InputLayout.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "Topology.h"
#include "TransformCbuf.h"
#include <memory>
Box::Box(Graphics& gfx)
{
	if (!IsStaticInitialized())
	{
		Model cube("obj\\cube.obj");

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, cube.GetVertices()));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, cube.GetIndices()));

		std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"Norm", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		auto pvs = std::make_unique<VertexShader>(gfx, L"cso\\VertexShader.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"cso\\PixelShader.cso"));

		struct PSConstantBuffer
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[6];
		};
		const PSConstantBuffer pscb =
		{
			{
				{1.0f,0.0f,1.0f},
				{1.0f,0.0f,0.0f},
				{0.0f,1.0f,0.0f},
				{0.0f,0.0f,1.0f},
				{1.0f,1.0f,0.0f},
				{0.0f,1.0f,1.0f},
			}
		};
		AddStaticBind(std::make_unique<PixelConstantBuffer<PSConstantBuffer>>(gfx, pscb));

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
	z_roll = dt * z_rot_speed;
	x_roll = dt * x_rot_speed;
	y_roll = dt * y_rot_speed;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationZ(z_roll) *
		DirectX::XMMatrixRotationY(y_roll) *
		DirectX::XMMatrixRotationX(x_roll) *
		DirectX::XMMatrixTranslation(m_pos_x, m_pos_y, m_pos_z);
}
