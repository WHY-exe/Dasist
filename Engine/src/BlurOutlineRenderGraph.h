#pragma once
#include "RenderGraph.h"
#include <memory>
#include "ConstantBufferEx.h"

class Graphics;
class Bindable;
class RenderTarget;

namespace Rgph
{
	class BlurOutlineRenderGraph : public RenderGraph
	{
	public:
		BlurOutlineRenderGraph( Graphics& gfx );
	private:
		// private functions
		void SetKernelGauss( int radius,float sigma ) noexcept(!IS_DEBUG);
		// private data
		static constexpr int maxRadius = 7;
		static constexpr int radius = 4;
		static constexpr float sigma = 5.0f;
		std::shared_ptr<CachingPixelConstantBuffer> blurKernel;
		std::shared_ptr<CachingPixelConstantBuffer> blurDirection;
	};
}
