#pragma once
#include <DirectXMath.h>
#include "ConstantBuffer.h"
class Light
{
private:
	struct LightCBuffer
	{
		DirectX::XMFLOAT3 lightPos;		
		float padding1;
		DirectX::XMFLOAT3 ambient;
		float padding2;
		DirectX::XMFLOAT3 diffuseColor;
		float padding3;
		float diffuseIntensity;
		float attConst;
		float attLinear;
		float attQuad;

	};
public:
	Light(Graphics& gfx);
	void Bind(Graphics& gfx) noexcept;
	void SpwanControlWindow() noexcept;
private:
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_Ambient = {0.25f, 0.25f, 0.25f};
	DirectX::XMFLOAT3 m_diffuseColor = {1.0f, 1.0f, 1.0f};
	float m_diffuseIntensity = 1.0f;
	float m_attConst = 1.0f;
	float m_attLinear = 1.0f;
	float m_attQuad = 1.0f;
	PixelConstantBuffer<LightCBuffer> m_PSCbuf;
};

