#pragma once
#include "Bindable.h"
#include "Camera.h"
#include "ConstantBuffer.h"
class ShadowCamCbuf : public Bindable
{
public:
public:
	struct Transform {
		DirectX::XMMATRIX Viewproj;
	};
public:
	ShadowCamCbuf(Graphics& gfx, UINT slot = 4u);
	void Bind(Graphics& gfx) noexcept(!IS_DEBUG) override;
	void Update(Graphics& gfx) const noexcept;
	void SetCamera(const Camera& cam) noexcept;
private:
	std::unique_ptr<VertexConstantBuffer<Transform>> m_vcbuf;
	const Camera* m_pCamera = nullptr;
};