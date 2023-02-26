#pragma once
#include "Drawable.h"
#include "ConstantBuffer.h"
#include <memory>
class TransformCbuf:public Bindable
{
public:
	struct Transform {
		DirectX::XMMATRIX modelCamView;
		DirectX::XMMATRIX modelProjView;
		DirectX::XMMATRIX mtx_scalin;
		DirectX::XMFLOAT3 CenterPoint;
	};
public:
	TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot = 0);
	void SetScalin(float scale_in) noexcept;
	void Bind(Graphics& gfx) noexcept override;
protected:
	Transform GetTransform(Graphics& gfx) const noexcept;
	void UpdateBindImpl(Graphics& gfx, const Transform& tf) const noexcept;
private:
	static std::unique_ptr<VertexConstantBuffer<Transform>> s_vcbuf;
	float m_scalein = 1.0f;
	const Drawable& m_parent;
};

