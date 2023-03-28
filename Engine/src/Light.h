#pragma once
#include <DirectXMath.h>
#include "ConstantBufferEx.h"
#include "LightIndicator.h"
#include "DynamicConstantBuffer.h"
#include <memory>
class PointLight
{
public:
	PointLight(Graphics& gfx);
	void Update(Graphics& gfxF) noexcept;
	void Submit() noexcept;
	void LinkTechniques(Rgph::RenderGraph& rg) noexcept(!IS_DEBUG);
	void SpwanControlWindow() noexcept;
private:
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_ambient = {0.001f, 0.001f, 0.001f};
	DirectX::XMFLOAT3 m_diffuseColor = {1.0f, 1.0f, 1.0f};
	float m_diffuseIntensity = 1.0f;
	float m_attConst = 0.0001f;
	float m_attLinear = 0.0001f;
	float m_attQuad = 0.00005f;
	BOOL m_Enable = TRUE;
	std::unique_ptr<CachingPixelConstantBuffer> m_PSCbuf;
	DCBuf::Buffer m_cBuffer;
	LightIndicator m_indicator;
};

class GlobalLight
{
public:
	GlobalLight(Graphics& gfx);
	void Update(Graphics& gfx) noexcept;
	void SpwanControlWindow() noexcept;
private:
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_rot;
	DirectX::XMFLOAT3 m_ambient = { 0.01f, 0.01f, 0.01f };;
	DirectX::XMFLOAT3 m_diffuseColor = { 1.0f, 1.0f, 1.0f };
	float m_diffuseIntensity = 1.0f;
	BOOL m_Enable = TRUE;
	std::unique_ptr<CachingPixelConstantBuffer> m_PSCbuf;
	DCBuf::Buffer m_cBuffer;
};