#pragma once
#include "RenderQueuePass.h"
#include "DepthStencil.h"
#include "Stencil.h"
#include "Signal.h"
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
			AddBindSink<Bindable>("shadowRasterizer");
			RegisterSource(DirectBindableSource<DepthStencil>::Make("ShadowMap", depthStencil));
		}
		void BindCamera(std::shared_ptr<Camera> cam)
		{
			m_pShadowCamera = std::move(cam);
		}
		void Execute(Graphics& gfx) const noexcept(!IS_DEBUG) override
		{
			depthStencil->Clear(gfx);
			// TODO : resize the depth map when the window's size is changing
			SIGNAL(
				gfx.sizeSignalSM,
				depthStencil->Resize(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight())
			);
			m_pShadowCamera->BindtoGFX(gfx);
			RenderQueuePass::Execute(gfx);
		}
		void DumpShadowMap(Graphics& gfx, const std::string& path)
		{
			depthStencil->ToSurface(gfx).SaveAsFile(path);
		}
	private:
		std::shared_ptr<Camera> m_pShadowCamera;
	};

}