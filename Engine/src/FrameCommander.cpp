#include "FrameCommander.h"
#include "NullPixelShader.h"
#include "ConstantBuffer.h"
#include "Vertex.h"
#include "Stencil.h"
FrameCommander::FrameCommander(Graphics& gfx)
	:
	m_ds(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight()),
	m_rt1({ gfx, gfx.GetWindowWidth() / (UINT)m_downFactor, gfx.GetWindowHeight() / (UINT)m_downFactor }),
	m_rt2({ gfx, gfx.GetWindowWidth() / (UINT)m_downFactor, gfx.GetWindowHeight() / (UINT)m_downFactor }),
	m_blur_pack(gfx, L"res\\cso\\BlurOutlinePs.cso")
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
	m_pvsFS = VertexShader::Resolve(gfx, L"res\\cso\\Full_VS.cso");
	m_pilFS = InputLayout::Resolve(gfx, vl, m_pvsFS->GetByteCode());
	m_pSamplerPoint = Sampler::Resolve(gfx, Sampler::Type::Point, true);
	m_pSamplerBiLinear = Sampler::Resolve(gfx, Sampler::Type::Bilinear, true);
}
void FrameCommander::Accept(const Job& job, size_t target) const noexcept
{
	m_Passes[target].Accept(job);
}

void FrameCommander::Execute(Graphics& gfx) const noexcept(!IS_DEBUG)
{	
	// Set up render target for normal passes
	m_ds.Clear(gfx);
	// bind the normal drawing to the render_target1
	m_rt1->Clear(gfx);
	gfx.BindSwapBuffer(m_ds);
	if (gfx.m_bIsSizeChanged)
	{
		m_ds.Resize(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight());
		m_rt1->Resize(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight());
		m_rt2->Resize(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight());
		gfx.m_bIsSizeChanged = false;
	}
	// main phong lighting pass
	Blender::Resolve(gfx, false)->Bind(gfx);
	m_Passes[0].Execute(gfx);
	// outline masking pass
	m_Passes[1].Execute(gfx);
	// outline drawing pass
	// use the m_rt as back buffer
	m_rt1->BindAsTarget(gfx);
	m_Passes[2].Execute(gfx);
	// bind the h-blur effect to render_target2
	m_rt2->BindAsTarget(gfx);
	m_rt1->BindAsTexture(gfx, 0u);
	m_pvbFS->Bind(gfx);
	m_pibFS->Bind(gfx);
	m_pvsFS->Bind(gfx);
	m_pilFS->Bind(gfx);
	m_pSamplerPoint->Bind(gfx);
	m_blur_pack.Bind(gfx);
	m_blur_pack.SetHorizontal(gfx);
	// draw the gauss blur effect
	gfx.DrawIndexed(m_pibFS->GetSize());
	// fullscreen v-blur + blend pass
	gfx.BindSwapBuffer(m_ds);
	m_rt2->BindAsTexture(gfx, 0u);
	Blender::Resolve(gfx, true)->Bind(gfx);
	m_pSamplerBiLinear->Bind(gfx);
	Stencil::Resolve(gfx, Stencil::Mod::Mask)->Bind(gfx);
	m_blur_pack.SetVertical(gfx);
	gfx.DrawIndexed(m_pibFS->GetSize());
}

void FrameCommander::ShowFliterControl(Graphics& gfx) noexcept(!IS_DEBUG)
{
	if (ImGui::Begin("Blur"))
	{
		if (ImGui::SliderInt("DownFactor", &m_downFactor, 1, 10))
		{
			m_rt1.emplace(gfx, gfx.GetWindowWidth() / (UINT)m_downFactor, gfx.GetWindowHeight() / (UINT)m_downFactor);
			m_rt2.emplace(gfx, gfx.GetWindowWidth() / (UINT)m_downFactor, gfx.GetWindowHeight() / (UINT)m_downFactor);
		}
		m_blur_pack.RenderWidgets(gfx);
	}
	ImGui::End();
}

void FrameCommander::Reset() noexcept
{
	for (auto& p : m_Passes)
	{
		p.Reset();
	}
}
