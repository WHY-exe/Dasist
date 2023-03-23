#include "BlurOutlineRenderGraph.h"
#include "BufferClearPass.h"
#include "LambertianPass.h"
#include "OutlineDrawingPass.h"
#include "OutlineMaskGenerationPass.h"
#include "Source.h"
#include "HorizontalBlurPass.h"
#include "VerticalBlurPass.h"
#include "BlurOutlineDrawingPass.h"
#include "RenderTarget.h"
#include "DynamicConstantBuffer.h"
#include <imgui.h>
#include "MathTool.h"
namespace Rgph
{
	BlurOutlineRenderGraph::BlurOutlineRenderGraph( Graphics& gfx )
		:
		RenderGraph( gfx )
	{
		{
			auto pass = std::make_unique<BufferClearPass>( "clearRT" );
			pass->SetSinkLinkage( "buffer","$.backbuffer" );
			AppendPass( std::move( pass ) );
		}
		{
			auto pass = std::make_unique<BufferClearPass>( "clearDS" );
			pass->SetSinkLinkage( "buffer","$.masterDepth" );
			AppendPass( std::move( pass ) );
		}
		{
			auto pass = std::make_unique<LambertianPass>( gfx,"lambertian" );
			pass->SetSinkLinkage( "renderTarget","clearRT.buffer" );
			pass->SetSinkLinkage( "depthStencil","clearDS.buffer" );
			AppendPass( std::move( pass ) );
		}
		{
			auto pass = std::make_unique<OutlineMaskGenerationPass>( gfx,"outlineMask" );
			pass->SetSinkLinkage( "depthStencil","lambertian.depthStencil" );
			AppendPass( std::move( pass ) );
		}

		// setup blur constant buffers
		{
			{
				DCBuf::RawLayout l;
				l.Add<DCBuf::Integer>( "nTaps" );
				l.Add<DCBuf::Array>( "coefficients" );
				l["coefficients"].Set<DCBuf::Float>( maxRadius * 2 + 1 );
				DCBuf::Buffer buf{ std::move( l ) };
				blurKernel = std::make_shared<CachingPixelConstantBuffer>( gfx,buf,0 );
				SetKernelGauss( radius,sigma );
				AddGlobalSource( DirectBindableSource<CachingPixelConstantBuffer>::Make( "blurKernel",blurKernel ) );
			}
			{
				DCBuf::RawLayout l;
				l.Add<DCBuf::Bool>( "isHorizontal" );
				DCBuf::Buffer buf{ std::move( l ) };
				blurDirection = std::make_shared<CachingPixelConstantBuffer>( gfx,buf,1 );
				AddGlobalSource( DirectBindableSource<CachingPixelConstantBuffer>::Make( "blurDirection",blurDirection ) );
			}
		}

		{
			auto pass = std::make_unique<BlurOutlineDrawingPass>( gfx,"outlineDraw",gfx.GetWindowWidth(),gfx.GetWindowHeight() );
			AppendPass( std::move( pass ) );
		}
		{
			auto pass = std::make_unique<HorizontalBlurPass>( "horizontal",gfx,gfx.GetWindowWidth(),gfx.GetWindowHeight() );
			pass->SetSinkLinkage( "scratchIn","outlineDraw.scratchOut" );
			pass->SetSinkLinkage( "kernel","$.blurKernel" );
			pass->SetSinkLinkage( "direction","$.blurDirection" );
			AppendPass( std::move( pass ) );
		}
		{
			auto pass = std::make_unique<VerticalBlurPass>( "vertical",gfx );
			pass->SetSinkLinkage( "renderTarget","lambertian.renderTarget" );
			pass->SetSinkLinkage( "depthStencil","outlineMask.depthStencil" );
			pass->SetSinkLinkage( "scratchIn","horizontal.scratchOut" );
			pass->SetSinkLinkage( "kernel","$.blurKernel" );
			pass->SetSinkLinkage( "direction","$.blurDirection" );
			AppendPass( std::move( pass ) );
		}
		SetSinkTarget( "backbuffer","vertical.renderTarget" );

		Finalize();
	}

	void BlurOutlineRenderGraph::SetKernelGauss( int radius,float sigma ) noexcept(!_DEBUG)
	{
		assert( radius <= maxRadius );
		auto k = blurKernel->GetBuffer();
		const int nTaps = radius * 2 + 1;
		k["nTaps"] = nTaps;
		float sum = 0.0f;
		for( int i = 0; i < nTaps; i++ )
		{
			const auto x = float( i - radius );
			const auto g = math_tool::gauss( x,sigma );
			sum += g;
			k["coefficients"][i] = g;
		}
		for( int i = 0; i < nTaps; i++ )
		{
			k["coefficients"][i] = (float)k["coefficients"][i] / sum;
		}
		blurKernel->SetBuffer( k );
	}
	void BlurOutlineRenderGraph::SetKernelBox(int radius) noexcept(!_DEBUG)
	{
		assert(radius <= maxRadius);
		auto k = blurKernel->GetBuffer();
		const int nTaps = radius * 2 + 1;
		k["nTaps"] = nTaps;
		const float c = 1.0f / nTaps;
		for (int i = 0; i < nTaps; i++)
		{
			k["coefficients"][i] = c;
		}
		blurKernel->SetBuffer(k);
	}

	void BlurOutlineRenderGraph::RenderWidgets(Graphics& gfx)
	{
		if (ImGui::Begin("Kernel"))
		{
			bool filterChanged = false;
			{
				const char* items[] = { "Gauss","Box" };
				static const char* curItem = items[0];
				if (ImGui::BeginCombo("Filter Type", curItem))
				{
					for (int n = 0; n < std::size(items); n++)
					{
						const bool isSelected = (curItem == items[n]);
						if (ImGui::Selectable(items[n], isSelected))
						{
							filterChanged = true;
							curItem = items[n];
							if (curItem == items[0])
							{
								m_KernelType = KernelType::Gauss;
							}
							else if (curItem == items[1])
							{
								m_KernelType = KernelType::Box;
							}
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
			}

			bool radChange = ImGui::SliderInt("Radius", &radius, 0, maxRadius);
			bool sigChange = ImGui::SliderFloat("Sigma", &sigma, 0.1f, 10.0f);
			if (radChange || sigChange || filterChanged)
			{
				if (m_KernelType == KernelType::Gauss)
				{
					SetKernelGauss(radius, sigma);
				}
				else if (m_KernelType == KernelType::Box)
				{
					SetKernelBox(radius);
				}
			}
		}
		ImGui::End();
	}
}
