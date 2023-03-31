#pragma once
#include <DirectXMath.h>
#include "ConstantBufferEx.h"
#include "LightIndicator.h"
#include "DynamicConstantBuffer.h"
#include <memory>
class Light
{
	friend class LightContainer;
public:
	Light(std::string szName, int index) noexcept;
	const std::string& GetName() const noexcept;
	virtual void Update() noexcept = 0;
	virtual void Submit() noexcept = 0 ;
	virtual void LinkTechniques(Rgph::RenderGraph& rg) noexcept(!IS_DEBUG) = 0;
	virtual void SpwanControlWidgets() noexcept;
protected:	
	int m_index;
	std::string m_szName;
	BOOL m_Enable = TRUE;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_ambient = { 0.001f, 0.001f, 0.001f };
	DirectX::XMFLOAT3 m_diffuseColor = { 1.0f, 1.0f, 1.0f };	
	float m_diffuseIntensity = 1.0f;
};
class PointLight :public Light
{
	friend class LightContainer;
public:
	PointLight(Graphics& gfx, int index);
	void Update() noexcept override;
	void Submit() noexcept override;
	void LinkTechniques(Rgph::RenderGraph& rg) noexcept(!IS_DEBUG) override;
	void SpwanControlWidgets() noexcept override;
private:
	bool m_showIndicator = true;
	float m_attConst = 0.0001f;
	float m_attLinear = 0.0001f;
	float m_attQuad = 0.00005f;
	LightIndicator m_indicator;
};

class GlobalLight : public Light
{
	friend class LightContainer;
public:
	GlobalLight(std::string szName, int index) noexcept
		:
		Light("GlobalLight" + std::to_string(index), index)
	{};
	void Update() noexcept override{};
	void Submit() noexcept override{};
	void LinkTechniques(Rgph::RenderGraph& rg) noexcept(!IS_DEBUG) override {};
	void SpwanControlWidgets() noexcept override
	{
		Light::SpwanControlWidgets();
	};
private:
	float Row, Yaw, Pitch;
};