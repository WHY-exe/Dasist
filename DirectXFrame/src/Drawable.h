#pragma once
#include "Bindable.h"
#include "IndexBuffer.h"
#include <memory>
#include <DirectXMath.h>
class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	void Draw(Graphics& gfx);
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	virtual void Update(float dt) noexcept = 0;
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept;
	virtual ~Drawable() = default;
private:
	const IndexBuffer* m_pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> m_binds;
};

