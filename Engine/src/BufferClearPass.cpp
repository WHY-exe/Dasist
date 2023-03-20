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
		if (gfx.m_bIsSizeChanged)
		{
			if (auto rt = dynamic_cast<RenderTarget*>(buffer.get()))
			{
				rt->Resize(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight());
			}
			if (auto ds = dynamic_cast<DepthStencil*>(buffer.get()))
			{
				ds->Resize(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight());
			}
			gfx.m_bIsSizeChanged = false;
		}
		buffer->Clear( gfx );
	}
}