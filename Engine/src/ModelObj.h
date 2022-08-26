#pragma once
#include "Drawable.h"
#include "ConstantBuffer.h"
#include "Surface.h"
#include <memory>
// don't use the different constuctor for the same model name
// the different constructo will generate differnt static buffer
class ModelObj : public Drawable
{
private:
	struct ModelCBuffer
	{
		alignas(16) DirectX::XMFLOAT3 m_Ambient;
		float specular_intesity;
		float specular_pow;
	};
public:
	ModelObj(Graphics& gfx, const Surface& szTexPath, const std::string& szObjPath, const std::string& szModelName = "Object",
		const std::wstring& szPSPath = L"res\\cso\\TexPS.cso", const std::wstring& szVSPath = L"res\\cso\\TexVS.cso");
	ModelObj(Graphics& gfx, const std::string& szObjPath, const std::string& szModelName = "Object",
		const std::wstring& szPSPath = L"res\\cso\\PixelShader.cso", const std::wstring& szVSPath = L"res\\cso\\TexVS.cso");
	void Update(Graphics& gfx) noexcept override;
	void SpwanControlWindow() noexcept override;
	const std::string& GetModelName() const noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SetPos(float x, float y, float z) noexcept;
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	void Scale(float scale) noexcept;
private:
	void GenerateName() noexcept;
private:
	std::string m_szObjName;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_rot;
	DirectX::XMFLOAT3 m_Ambient;
	PixelConstantBuffer<ModelCBuffer> m_PSCbuf;
	float m_Scale = 1.0f;
	UINT m_uObjId;
	static UINT m_uObjNum;
	static std::map<std::string, UINT> m_NameMarker;
};

