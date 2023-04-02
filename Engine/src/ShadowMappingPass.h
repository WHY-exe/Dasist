#pragma once
#include "RenderQueuePass.h"
#include "DepthStencil.h"
#include "Stencil.h"
#include "Blender.h"
#include "VertexShader.h"
#include "NullPixelShader.h"
#include "Camera.h"
namespace Rgph
{
	class ShadowMappingPass : public RenderQueuePass
	{
	public:
		ShadowMappingPass(Graphics& gfx, std::string name)
			:
			RenderQueuePass(std::move(name))
		{
			depthStencil = std::make_unique<DepthStencilAsShaderInput>(gfx, 4u, DepthStencil::Usage::DepthForShadow);
			AddBind(VertexShader::Resolve(gfx, L"res\\cso\\Solid_VS.cso"));
			AddBind(NullPixelShader::Resolve(gfx));
			AddBind(Stencil::Resolve(gfx, Stencil::Mod::Off));
			AddBind(Blender::Resolve(gfx, false));
			RegisterSource(DirectBindableSource<DepthStencil>::Make("ShadowMap", depthStencil));
		}
		void Execute(Graphics& gfx) const noexcept(!IS_DEBUG) override
		{
			depthStencil->Clear(gfx);
			RenderQueuePass::Execute(gfx);
		}
	};

}