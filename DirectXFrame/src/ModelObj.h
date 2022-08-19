#pragma once
#include "DrawableBase.h"
#include "ConstantBuffer.h"
#include "Surface.h"
#include <memory>
// don't use the different constuctor for the same model name
// the different constructo will generate differnt static buffer
class ModelObj : public DrawableBase
{
private:
	struct ModelCBuffer
	{
		alignas(16) DirectX::XMFLOAT3 m_Ambient;
	};
public:
	ModelObj(Graphics& gfx, const Surface& szTexPath, const std::string& szObjPath, const std::string& szModelName = "Object",
		const std::wstring& szPSPath = L"cso//TexPS.cso", const std::wstring& szVSPath = L"cso//TexVS.cso");
	ModelObj(Graphics& gfx, const std::string& szObjPath, const std::string& szModelName = "Object",
		const std::wstring& szPSPath = L"cso//PixelShader.cso", const std::wstring& szVSPath = L"cso//TexVS.cso");
	void Update(Graphics& gfx) noexcept override;
	void SpwanControlWindow() noexcept override;
	const std::string& GetModelName() const noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	void GenerateName() noexcept;
private:
	std::string m_szObjName;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_rot;
	DirectX::XMFLOAT3 m_Ambient;
	PixelConstantBuffer<ModelCBuffer> m_PSCbuf;
	UINT m_uObjId;
	static UINT m_uObjNum;
	static std::map<std::string, UINT> m_NameMarker;
};

