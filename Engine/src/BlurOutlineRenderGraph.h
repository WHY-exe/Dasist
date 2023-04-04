#pragma once
#include "RenderGraph.h"
#include <memory>
#include "ConstantBufferEx.h"

class Graphics;
class Bindable;
class RenderTarget;
class Camera;
class LightContainer;
namespace Rgph
{
	class BlurOutlineRenderGraph : public RenderGraph
	{
	public:
		BlurOutlineRenderGraph( Graphics& gfx );		
		void RenderWidgets(Graphics& gfx);
		void DumpShadowMap(Graphics& gfx, const std::string& path);
		void BindLambertianCamera(const Camera& cam) noexcept(!IS_DEBUG);
		void BindShadowCamera(const LightContainer& lights) noexcept(!IS_DEBUG);
		void BindAddedShadowCamera(std::shared_ptr<Camera> cam) noexcept(!IS_DEBUG);
	private:
		// private functions
		void SetKernelGauss( int radius,float sigma ) noexcept(!IS_DEBUG);
		void SetKernelBox(int radius) noexcept(!IS_DEBUG);
	private:
		enum class KernelType
		{
			Gauss, Box
		} m_KernelType = KernelType::Gauss;
		// private data
		static constexpr int maxRadius = 7;
		int radius = 2;
		float sigma = 5.0f;
		std::shared_ptr<CachingPixelConstantBuffer> blurKernel;
		std::shared_ptr<CachingPixelConstantBuffer> blurDirection;

	};
}
