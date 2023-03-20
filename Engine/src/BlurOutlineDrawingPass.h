#pragma once
#include "RenderQueuePass.h"
#include "Job.h"
#include <vector>
#include "PixelShader.h"
#include "VertexShader.h"
#include "Stencil.h"
#include "Rasterizer.h"
#include "Source.h"
#include "RenderTarget.h"
#include "Blender.h"

class Graphics;

namespace Rgph
{
	class BlurOutlineDrawingPass : public RenderQueuePass
	{
	public:
		BlurOutlineDrawingPass( Graphics& gfx,std::string name,unsigned int fullWidth,unsigned int fullHeight )
			:
			RenderQueuePass( std::move( name ) )
		{
			renderTarget = std::make_unique<RenderTargetAsShaderTexture>( gfx,fullWidth / 2,fullHeight / 2,0 );
			AddBind( VertexShader::Resolve( gfx,L"res\\cso\\Solid_VS.cso" ) );
			AddBind( PixelShader::Resolve( gfx,L"res\\cso\\Solid_PS.cso" ) );
			AddBind( Stencil::Resolve( gfx,Stencil::Mod::Mask ) );
			AddBind( Blender::Resolve( gfx,false ) );
			RegisterSource( DirectBindableSource<RenderTarget>::Make( "scratchOut",renderTarget ) );
		}
		void Execute( Graphics& gfx ) const noexcept(!IS_DEBUG) override
		{
			renderTarget->Clear( gfx );
			RenderQueuePass::Execute( gfx );
		}
	};
}