#pragma once
#include "Graphics.h"
#include "ConstantBuffer.h"
#include "PixelShader.h"
#include <imgui.h>
#include <DirectXMath.h>
class BlurPack
{
public:
	BlurPack(Graphics& gfx, const std::wstring& shader, int radius = 7, float sigma = 1.0f) noexcept;
	void Bind(Graphics& gfx) noexcept(!IS_DEBUG);
	void SetVertical(Graphics& gfx) noexcept;
	void SetHorizontal(Graphics& gfx) noexcept;
	void SetGaussKernel(Graphics& gfx, int radius, float sigma) noexcept;
	void SetBoxKernel(Graphics& gfx, int radius) noexcept;
	void RenderWidgets(Graphics& gfx) noexcept(!IS_DEBUG);
private:
	int m_radius;
	float m_sigma;
	enum class KernelType
	{
		Gauss, Box
	} m_KernelType = KernelType::Gauss;
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