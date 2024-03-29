#pragma once
#include "FullscreenPass.h"
#include "ConstantBufferEx.h"

class Graphics;
namespace Bind
{
	class PixelShader;
	class RenderTarget;
}

namespace Rgph
{
	class VerticalBlurPass : public FullscreenPass
	{
	public:
		VerticalBlurPass( std::string name,Graphics& gfx );
		void Execute( Graphics& gfx ) const noexcept(!IS_DEBUG) override;
	private:
		std::shared_ptr<CachingPixelConstantBuffer> direction;
	};
}