#include "Step.h"
#include "FrameCommander.h"
#include "Job.h"
Step::Step(size_t pass_index)
	:
	m_pass_index(pass_index)
{
}

void Step::AddBind(std::shared_ptr<Bindable> bind) noexcept
{
	m_Binds.push_back(std::move(bind));
}

void Step::SetActiveState(bool active) noexcept
{
	m_active_state = active;
}

bool Step::IsActive() const noexcept
{
	return m_active_state;
}

void Step::InitializeParentReference(const Drawable& d) noexcept
{
	for (auto& b : m_Binds)
	{
		b->InitializeParentReference(d);
	}
}

void Step::Submit(const FrameCommander& fc, const Drawable& d) const
{
	fc.Accept(Job(this, &d), m_pass_index);
}

void Step::Bind(Graphics& gfx) const
{
	for (auto& b : m_Binds)
	{
		b->Bind(gfx);
	}
}

void Step::Accept(Probe& probe) noexcept
{
	probe.SetStep(this);
	for (auto& i : m_Binds)
	{
		i->Accept(probe);
	}
}
