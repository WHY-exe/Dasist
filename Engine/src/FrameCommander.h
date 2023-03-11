#pragma once
#include "Pass.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "IndexBuffer.h"
#include "Blender.h"
#include "Sampler.h"
#include "InputLayout.h"
#include <memory>
#include <array>
class FrameCommander
{
public:
	FrameCommander(Graphics& gfx);
	void Accept(const Job& job, size_t target) const noexcept;
	void Execute(Graphics& gfx) const noexcept(!IS_DEBUG);
	void Reset() noexcept;
private:
	mutable RenderTarget m_rt;
	mutable DepthStencil m_ds;
	mutable std::array<Pass, 3> m_Passes;

	std::shared_ptr<VertexBuffer> m_pvbFS;
	std::shared_ptr<IndexBuffer> m_pibFS;
	std::shared_ptr<VertexShader> m_pvsFS;
	std::shared_ptr<PixelShader> m_ppsFS;
	std::shared_ptr<InputLayout> m_pilFS;
	std::shared_ptr<Sampler> m_pSampler;
	std::shared_ptr<Blender> m_pBlender;
};