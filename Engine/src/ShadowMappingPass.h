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
			RegisterSource(DirectBufferSource<DepthStencil>::Make("ShadowMap", depthStencil));
		}
		void BindCamera(const Camera& cam)
		{
			m_pCamera = &cam;
		}
		void Execute(Graphics& gfx) const noexcept(!IS_DEBUG) override
		{
			assert(m_pCamera);
			depthStencil->Clear(gfx);
			m_pCamera->BindtoGFX(gfx);
			RenderQueuePass::Execute(gfx);
		}
	private:
		const Camera* m_pCamera;
	};

}