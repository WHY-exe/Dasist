#pragma once
#include "DrawableBase.h"
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
#include "Model.h"
#include <memory>

template <class C>
class ModelObj : public DrawableBase<C>
{
private:
	struct ModelCBuffer
	{
		alignas(16) DirectX::XMFLOAT3 m_Ambient;
	};
protected:
	ModelObj(
		const Model& obj, Graphics& gfx, const Surface& tex,
		const std::wstring& szVSPath, const std::wstring& szPSPath
	)
		:
		m_pos(0.0f, 0.0f, 0.0f),
		m_rot(0.0f, 0.0f, 0.0f),
		m_Ambient( 0.25f, 0.25f, 0.25f ),
		m_PSCbuf(gfx, 1u)
	{
		if (!DrawableBase<C>::IsStaticInitialized())
		{

			DrawableBase<C>::AddStaticBind(std::make_unique<VertexBuffer>(gfx, obj.GetVertices()));

			DrawableBase<C>::AddStaticBind(std::make_unique<Texture>(gfx, tex));

			DrawableBase<C>::AddStaticBind(std::make_unique<Sampler>(gfx));

			DrawableBase<C>::AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, obj.GetIndices()));

			auto pvs = std::make_unique<VertexShader>(gfx, szVSPath);
			auto pvsbc = pvs->GetByteCode();
			DrawableBase<C>::AddStaticBind(std::move(pvs));

			DrawableBase<C>::AddStaticBind(std::make_unique<PixelShader>(gfx, szPSPath));

			DrawableBase<C>::AddStaticBind(std::make_unique<InputLayout>(gfx, obj.GetVertexLayout(), pvsbc));

			DrawableBase<C>::AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else
		{
			DrawableBase<C>::SetIndexfromStatic();
		}

		Drawable::AddBind(std::make_unique<TransformCbuf>(gfx, *this));
	}
	void Update(Graphics& gfx) noexcept override
	{
		m_PSCbuf.Update(gfx, ModelCBuffer(m_Ambient));
		m_PSCbuf.Bind(gfx);
	}

	void SpwanControlWindow(const std::string& szObjName, UINT uObjId) noexcept
	{
		std::stringstream ss;
		ss << szObjName << uObjId;
		if (ImGui::Begin(ss.str().c_str()))
		{
			ImGui::Text("Position");
			ImGui::SliderFloat("X", &m_pos.x, -80.0f, 80.0f, "%.1f");
			ImGui::SliderFloat("Y", &m_pos.y, -80.0f, 80.0f, "%.1f");
			ImGui::SliderFloat("Z", &m_pos.z, -80.0f, 80.0f, "%.1f");
			ImGui::Text("Angle");
			ImGui::SliderAngle("AngleX", &m_rot.x, -180.0f, 180.0f, "%.1f");
			ImGui::SliderAngle("AngleY", &m_rot.y, -180.0f, 180.0f, "%.1f");
			ImGui::SliderAngle("AngleZ", &m_rot.z, -180.0f, 180.0f, "%.1f");
			ImGui::Text("Ambient");
			ImGui::ColorEdit3("AmbientColor", &m_Ambient.x);	
		}
		ImGui::End();
	}
protected:
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_rot;
	DirectX::XMFLOAT3 m_Ambient;
	PixelConstantBuffer<ModelCBuffer> m_PSCbuf;
};

