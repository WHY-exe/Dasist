#pragma once
#include "RenderQueuePass.h"
#include "Job.h"
#include <vector>
#include "Sink.h"
#include "Source.h"
#include "Stencil.h"
namespace Rgph
{
	class LambertianPass : public RenderQueuePass
	{
	public:
		LambertianPass( Graphics& gfx,std::string name )
			:
			RenderQueuePass( std::move( name ) )
		{
			RegisterSink( DirectBufferSink<RenderTarget>::Make( "renderTarget",renderTarget ) );
			RegisterSink( DirectBufferSink<DepthStencil>::Make( "depthStencil",depthStencil ) );
			RegisterSource( DirectBufferSource<RenderTarget>::Make( "renderTarget",renderTarget ) );
			RegisterSource( DirectBufferSource<DepthStencil>::Make( "depthStencil",depthStencil ) );
			AddBind( Stencil::Resolve( gfx,Stencil::Mod::Off ) );
		}
	};
}