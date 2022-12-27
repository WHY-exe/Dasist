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
	// the normal phone shading draw call
	//Stencil::Resolve(gfx, Stencil::Mod::Off)->Bind(gfx);
	m_Passes[0].Excecute(gfx);
	//// outline draw call
	//Stencil::Resolve(gfx, Stencil::Mod::Write)->Bind(gfx);
	//m_Passes[1].Excecute(gfx);
	//// mask draw call 
	//Stencil::Resolve(gfx, Stencil::Mod::Mask)->Bind(gfx);
	//NullPixelShader::Resolve(gfx)->Bind(gfx);
	//struct SolidColorBuffer
	//{
	//	DirectX::XMFLOAT4 color = { 1.0f , 0.0f, 0.0f, 1.0f };
	//} scb;
	//PixelConstantBuffer<SolidColorBuffer>::Resolve(gfx, scb, 1u);
	//m_Passes[2].Excecute(gfx);
}

void FrameCommander::Reset() noexcept
{
	for (auto& p : m_Passes)
	{
		p.Reset();
	}
}
