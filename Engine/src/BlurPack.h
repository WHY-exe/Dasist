#pragma once
#include "Graphics.h"
#include "ConstantBuffer.h"
#include "PixelShader.h"
#include <imgui.h>
#include <DirectXMath.h>
class BlurPack
{
public:
	BlurPack(Graphics& gfx, int radius = 7, float sigma = 1.0f) noexcept;
	void Bind(Graphics& gfx) noexcept;
	void SetVertical(Graphics& gfx) noexcept;
	void SetHorizontal(Graphics& gfx) noexcept;
	void SetKernel(Graphics& gfx, int radius, float sigma) noexcept;
	void ShowWindow(Graphics& gfx) noexcept(!IS_DEBUG)
	{
		ImGui::Begin("Blur");
		bool radChange = ImGui::SliderInt("Radius", &m_radius, 0, 15);
		bool sigChange = ImGui::SliderFloat("Sigma", &m_sigma, 0.1f, 10.0f);
		if (radChange || sigChange)
		{
			SetKernel(gfx, m_radius, m_sigma);
		}
		ImGui::End();
	}
private:
	int m_radius;
	float m_sigma;
	struct Kernel
	{
		int nTabs;
		float paddiing[3];
		DirectX::XMFLOAT4 cofficients[31];
	};
	struct Control
	{
		BOOL horizontal;
		float paddiing[3];
	};
	PixelShader m_ps;
	PixelConstantBuffer<Kernel> m_Kernel;
	PixelConstantBuffer<Control> m_Control;
};