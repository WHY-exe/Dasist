#include "Drawable.h"

void Drawable::Draw(Graphics& gfx)
{
	for (auto& b: m_binds)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(m_pIndexBuffer->GetSize());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	m_binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
{
	assert("Attempting to add index buffer a second time" && m_pIndexBuffer == nullptr);
	m_pIndexBuffer = ibuf.get();
	m_binds.push_back(std::move(ibuf));
}