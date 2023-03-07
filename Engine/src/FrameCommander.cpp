#include "FrameCommander.h"
#include "NullPixelShader.h"
#include "ConstantBuffer.h"
#include "Stencil.h"
FrameCommander::FrameCommander(Graphics& gfx)
	:
	m_ds(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight())
{
}
void FrameCommander::Accept(const Job& job, size_t target) const noexcept
{
	m_Passes[target].Accept(job);
}

void FrameCommander::Excecute(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	m_ds.Clear(gfx);
	if (gfx.m_bIsSizeChanged)
	{
		m_ds.Resize(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight());
		gfx.m_bIsSizeChanged = false;
	}
	gfx.BindSwapBuffer(m_ds);
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
