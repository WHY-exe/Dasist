#pragma once
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "Scene.h"
class PointLight
{
private:
	struct PointLightCBuffer
	{
		alignas(16) DirectX::XMFLOAT3 lightPos;		
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLinear;
		float attQuad;

	};
public:
	PointLight(Graphics& gfx);
	void Update(Graphics& gfx) noexcept;
	void SpwanControlWindow() noexcept;
	void Draw(Graphics& gfx) noexcept;
private:
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_diffuseColor = {1.0f, 1.0f, 1.0f};
	float m_diffuseIntensity = 1.0f;
	float m_attConst = 1.0f;
	float m_attLinear = 1.0f;
	float m_attQuad = 1.0f;
	PixelConstantBuffer<PointLightCBuffer> m_PSCbuf;
	Scene::Model m_lightBall;
};

class GlobalLight
{
private:
	struct GlobalLightCBuffer
	{
		alignas(16) DirectX::XMFLOAT3 lightPos;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
	};
public:
	GlobalLight(Graphics& gfx);
	void Update(Graphics& gfx) noexcept;
	void SpwanControlWindow() noexcept;
private:
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_rot;
	DirectX::XMFLOAT3 m_diffuseColor = { 1.0f, 1.0f, 1.0f };
	float m_diffuseIntensity = 1.0f;
	PixelConstantBuffer<GlobalLightCBuffer> m_PSCbuf;
};