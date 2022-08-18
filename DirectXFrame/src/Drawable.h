#pragma once
#include "Bindable.h"
#include "IndexBuffer.h"
#include <memory>
#include <DirectXMath.h>
class Drawable
{
	template <class T>
	friend class DrawableBase;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	void Draw(Graphics& gfx);
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	virtual void SpwanControlWindow() noexcept = 0;
	virtual void Update(Graphics& gfx) noexcept = 0;
	virtual ~Drawable() = default;
protected:
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept;
protected:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;
private:
	const IndexBuffer* m_pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> m_binds;
};

