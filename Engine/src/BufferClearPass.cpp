#include "BufferClearPass.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "Sink.h"
#include "Source.h"


namespace Rgph
{
	BufferClearPass::BufferClearPass( std::string name )
		:
		Pass( std::move( name ) )
	{
		RegisterSink( DirectBufferSink<BufferResource>::Make( "buffer",buffer ) );
		RegisterSource( DirectBufferSource<BufferResource>::Make( "buffer",buffer ) );
	}

	void BufferClearPass::Execute( Graphics& gfx ) const noexcept(!IS_DEBUG)
	{
		buffer->Clear( gfx );
	}
}