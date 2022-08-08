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
#include <memory>
Box::Box(Graphics& gfx)
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
	z_roll = dt * z_rot_speed;
	x_roll = dt * x_rot_speed;
	y_roll = dt * y_rot_speed;
}

void Box::SetPosition(float x, float y, float z)
{
	m_pos_x = x;
	m_pos_y = y;
	m_pos_z = z;
}

void Box::SetRotSpeed(float x_srot, float y_srot, float z_srot)
{
	x_rot_speed	= x_srot;
	y_rot_speed	= y_srot;
	z_rot_speed	= z_srot;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationZ(z_roll) *
		DirectX::XMMatrixRotationY(y_roll) *
		DirectX::XMMatrixRotationX(x_roll) *
		DirectX::XMMatrixTranslation(m_pos_x, m_pos_y, m_pos_z);
}
