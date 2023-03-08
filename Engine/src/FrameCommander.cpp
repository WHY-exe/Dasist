#include "FrameCommander.h"
#include "NullPixelShader.h"
#include "ConstantBuffer.h"
#include "Vertex.h"
#include "Stencil.h"
FrameCommander::FrameCommander(Graphics& gfx)
	:
	m_ds(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight()),
	m_rt(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight())
{
	Vertex::Layout vl;
	vl.Append(Vertex::Position2D);
	Vertex::DataBuffer vbuffer(vl);
	vbuffer.EmplaceBack(DirectX::XMFLOAT2{ -1,1 });
	vbuffer.EmplaceBack(DirectX::XMFLOAT2{ 1,1 });
	vbuffer.EmplaceBack(DirectX::XMFLOAT2{ -1,-1 });
	vbuffer.EmplaceBack(DirectX::XMFLOAT2{ 1,-1 });
	m_pvbFS = VertexBuffer::Resolve(gfx, L"$Full", vbuffer);
	std::vector<UINT> indicies = { 0, 1, 2, 1, 3, 2};
	m_pibFS = IndexBuffer::Resolve(gfx, L"$Full", std::move(indicies));
	m_ppsFS = PixelShader::Resolve(gfx, L"res\\cso\\Full_PS.cso");
	m_pvsFS = VertexShader::Resolve(gfx, L"res\\cso\\Full_VS.cso");
	m_pilFS = InputLayout::Resolve(gfx, vl, m_pvsFS->GetByteCode());

}
void FrameCommander::Accept(const Job& job, size_t target) const noexcept
{
	m_Passes[target].Accept(job);
}

void FrameCommander::Excecute(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	m_ds.Clear(gfx);
	m_rt.Clear(gfx);
	m_rt.BindAsTarget(gfx, m_ds);
	if (gfx.m_bIsSizeChanged)
	{
		m_ds.Resize(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight());
		m_rt.Resize(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight());
		gfx.m_bIsSizeChanged = false;
	}
	for (auto& i : m_Passes)
	{
		i.Excecute(gfx);
	}
	gfx.BindSwapBuffer(m_ds);
	m_rt.BindAsTexture(gfx, 0u);
	m_pvbFS->Bind(gfx);
	m_pibFS->Bind(gfx);
	m_pvsFS->Bind(gfx);
	m_ppsFS->Bind(gfx);
	m_pilFS->Bind(gfx);
	gfx.DrawIndexed(m_pibFS->GetSize());
}

void FrameCommander::Reset() noexcept
{
	for (auto& p : m_Passes)
	{
		p.Reset();
	}
}
