#pragma once
#include "Drawable.h"
template <class T>
class DrawableBase : public Drawable
{
protected:
	bool IsStaticInitialized() const noexcept
	{
		return !m_staticBinds.empty();
	}
	void AddStaticBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG)
	{
		assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
		m_staticBinds.push_back(std::move(bind));
	}
	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> idx_buf) noexcept(!IS_DEBUG)
	{
		assert("Attempting to add index buffer a second time" && m_pIndexBuffer == nullptr);
		m_pIndexBuffer = idx_buf.get();
		m_staticBinds.push_back(std::move(idx_buf));
	}
	void SetIndexfromStatic() noexcept(!IS_DEBUG)
	{
		assert(m_pIndexBuffer == nullptr);
		for (auto& b: m_staticBinds)
		{
			if (const auto& p = dynamic_cast<IndexBuffer*>(b.get()))
			{ 
				m_pIndexBuffer = p;
				return;
			}
		}
		assert("fail to find index buffer from static binds" && m_pIndexBuffer != nullptr);
	}
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
	{
		return m_staticBinds;
	}
private:
	static std::vector<std::unique_ptr<Bindable>> m_staticBinds;
};

template <class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::m_staticBinds;