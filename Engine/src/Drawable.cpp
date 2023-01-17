#include "Drawable.h"
#include "TransformCbuf.h"
#include "ConstantBufferEx.h"

void Drawable::AddTechnique(Technique& tech) noexcept
{
	m_Techs.push_back(std::move(tech));
}

void Drawable::AddEssentialBind(std::shared_ptr<Bindable> bind) noexcept
{
	if (typeid(*bind) == typeid(IndexBuffer))
	{
		assert("Attempting to add index buffer a second time" && m_pIndexBuffer == nullptr);
		m_pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
	}
	m_essential_binds.push_back(std::move(bind));
}

void Drawable::Bind(Graphics& gfx) const noexcept
{
	for (auto& b : m_essential_binds)
	{
		b->Bind(gfx);
	}
}

void Drawable::Submit(FrameCommander& frameCommander) const noexcept
{
	for (auto& tech : m_Techs)
	{
		tech.Submit(frameCommander, *this);
	}
}

UINT Drawable::GetIndexSize() const noexcept
{
	return m_pIndexBuffer->GetSize();
}


