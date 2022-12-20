#include "Technique.h"
#include "Drawable.h"
void Technique::Submit(FrameCommander& fc, const Drawable& d) const noexcept
{
	if (m_activate)
	{
		for (auto& s : m_Steps)
		{
			s.Submit(fc, d);
		}
	}
}

void Technique::InitializeParentReference(const Drawable& d) noexcept
{
	for (auto& s : m_Steps)
	{
		s.InitializeParentReference(d);
	}
}

void Technique::AddStep(Step& step) noexcept
{
	m_Steps.push_back(std::move(step));
}

void Technique::Activate() noexcept
{
	m_activate = true;
}

void Technique::Deactivate() noexcept
{
	m_activate = false;
}
