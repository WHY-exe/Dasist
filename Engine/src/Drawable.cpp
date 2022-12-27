#include "Drawable.h"
#include "ConstantBufferEx.h"

void Drawable::AddTechnique(Technique& tech) noexcept
{
	m_Techs.push_back(std::move(tech));
}

void Drawable::AddVertexBuffer(std::shared_ptr<VertexBuffer> pvb) noexcept
{
	m_pVertexBuffer = pvb;
}

void Drawable::AddIndexBuffer(std::shared_ptr<IndexBuffer> pib) noexcept
{
	m_pIndexBuffer = pib;
}

void Drawable::AddTopology(std::shared_ptr<Topology> pt) noexcept
{
	m_pTopology = pt;
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


