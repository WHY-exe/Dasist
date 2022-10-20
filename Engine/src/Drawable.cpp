#include "Drawable.h"

void Drawable::Draw(Graphics& gfx)
{
	for (auto& b: m_binds)
	{
		if (b.get() != nullptr)
		{
			b->Bind(gfx);
		}
	}
	gfx.DrawIndexed(m_pIndexBuffer->GetSize());
}

void Drawable::AddBind(std::shared_ptr<Bindable> bind) noexcept(!IS_DEBUG)
{
	if (typeid(*bind) == typeid(IndexBuffer))
	{
		assert("Attempting to add index buffer a second time" && m_pIndexBuffer == nullptr);
		m_pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
	}
	m_binds.push_back(std::move(bind));
}
