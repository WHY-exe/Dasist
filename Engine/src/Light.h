#pragma once
#include <DirectXMath.h>
#include "ConstantBufferEx.h"
#include "LightIndicator.h"
#include "DynamicConstantBuffer.h"
#include <memory>
class PointLight
{
	friend class LightContainer;
public:
	PointLight(Graphics& gfx, int index);
	const std::string& GetName() const noexcept;
	void Update() noexcept;
	void Submit() noexcept;
	void LinkTechniques(Rgph::RenderGraph& rg) noexcept(!IS_DEBUG);
	void SpwanControlWidgets() noexcept;
private:
	size_t m_index;
	std::string m_szName;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_ambient = {0.001f, 0.001f, 0.001f};
	DirectX::XMFLOAT3 m_diffuseColor = {1.0f, 1.0f, 1.0f};
	float m_diffuseIntensity = 1.0f;
	float m_attConst = 0.0001f;
	float m_attLinear = 0.0001f;
	float m_attQuad = 0.00005f;
	DCBuf::Buffer m_PL_CBuffer;
	std::unique_ptr<CachingPixelConstantBuffer> m_PL_PSCbuf;
	BOOL m_Enable = TRUE;
	LightIndicator m_indicator;
};

class GlobalLight
{
	friend class LightContainer;
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