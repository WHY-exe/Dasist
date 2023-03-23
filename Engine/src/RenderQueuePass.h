#pragma once
#include "BindingPass.h"
#include "Job.h"
#include <vector>

namespace Rgph
{
	class RenderQueuePass : public BindingPass
	{
	public:
		using BindingPass::BindingPass;
		void Accept( Job job ) noexcept;
		void Execute( Graphics& gfx ) const noexcept(!_DEBUG) override;
		void Reset() noexcept(!_DEBUG) override;
	private:
		std::vector<Job> jobs;
	};
}