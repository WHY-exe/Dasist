#include "Drawable.h"
#include "ConstantBufferEx.h"

void Drawable::AddTechnique(Technique& tech) noexcept
{
	m_Techs.push_back(std::move(tech));
}

void Drawable::Bind(Graphics& gfx) const noexcept
{
	m_pVertexBuffer->Bind(gfx);
	m_pIndexBuffer->Bind(gfx);
	m_pTopology->Bind(gfx);
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


