#pragma once
#include "Bindable.h"
#include "IndexBuffer.h"
#include <map>
#include <memory>
#include <DirectXMath.h>
class Drawable
{
	friend class DrawableBase;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	void Draw(Graphics& gfx);
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	virtual ~Drawable() = default;
	template <typename T>
	T* QueryBindable() noexcept
	{
		for (auto& i : m_binds)
		{
			if (auto pt = dynamic_cast<T*>(i.get()))
			{
				return pt;
			}
		}
		return nullptr;
	}

protected:
	void AddBind(std::shared_ptr<Bindable> bind) noexcept(!IS_DEBUG);
private:
	const IndexBuffer* m_pIndexBuffer = nullptr;
	std::vector<std::shared_ptr<Bindable>> m_binds;
};

