#include "FrameCommander.h"
#include "NullPixelShader.h"
#include "ConstantBuffer.h"
#include "Stencil.h"
void FrameCommander::Accept(const Job& job, size_t target) const noexcept
{
	m_Passes[target].Accept(job);
}

void FrameCommander::Excecute(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	for (auto& i : m_Passes)
	{
		i.Excecute(gfx);
	}
}

void FrameCommander::Reset() noexcept
{
	for (auto& p : m_Passes)
	{
		p.Reset();
	}
}
