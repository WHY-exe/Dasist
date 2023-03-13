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
	std::vector<UINT> indicies = { 0, 1, 2, 1, 3, 2 };
	m_pibFS = IndexBuffer::Resolve(gfx, L"$Full", std::move(indicies));
	m_ppsFS = PixelShader::Resolve(gfx, L"res\\cso\\Full_PS.cso");
	m_pvsFS = VertexShader::Resolve(gfx, L"res\\cso\\Full_VS.cso");
	m_pilFS = InputLayout::Resolve(gfx, vl, m_pvsFS->GetByteCode());
	m_pSampler = Sampler::Resolve(gfx, false, true);
	m_pBlender = Blender::Resolve(gfx, true);
}
void FrameCommander::Accept(const Job& job, size_t target) const noexcept
{
	m_Passes[target].Accept(job);
}

void FrameCommander::Execute(Graphics& gfx) const noexcept(!IS_DEBUG)
{	
	// Set up render target for normal passes
	m_ds.Clear(gfx);
	m_rt.Clear(gfx);
	gfx.BindSwapBuffer(m_ds);
	if (gfx.m_bIsSizeChanged)
	{
		m_ds.Resize(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight());
		m_rt.Resize(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight());
		gfx.m_bIsSizeChanged = false;
	}
	// main phong lighting pass
	Blender::Resolve(gfx, false)->Bind(gfx);
	m_Passes[0].Execute(gfx);
	// outline masking pass
	m_Passes[1].Execute(gfx);
	// outline drawing pass
	// use the m_rt as back buffer
	m_rt.BindAsTarget(gfx);
	m_Passes[2].Execute(gfx);
	// fullscreen blur + blend pass
	// use the render target in the gfx as front buffer
	gfx.BindSwapBuffer(m_ds);
	m_rt.BindAsTexture(gfx, 0u);
	m_pvbFS->Bind(gfx);
	m_pibFS->Bind(gfx);
	m_pvsFS->Bind(gfx);
	m_ppsFS->Bind(gfx);
	m_pilFS->Bind(gfx);
	m_pSampler->Bind(gfx);
	m_pBlender->Bind(gfx);	
	Stencil::Resolve(gfx, Stencil::Mod::Mask)->Bind(gfx);	
	// 在这里绘制的是第三通道的纯色绘制的模糊效果
	gfx.DrawIndexed(m_pibFS->GetSize());
}

void FrameCommander::Reset() noexcept
{
	for (auto& p : m_Passes)
	{
		p.Reset();
	}
}
