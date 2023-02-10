#include "Technique.h"
#include "Drawable.h"
Technique::Technique(const std::string& name) noexcept
	:
	m_Name(name)
{
}
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

void Technique::SetActiveState(bool is_active) noexcept
{
	m_activate = is_active;
}

bool Technique::IsActive() const noexcept
{
	return m_activate;
}

void Technique::Accept(TechProbe& probe) noexcept
{
	for (auto& i:m_Steps)
	{
		i.Accept(probe);
	}
}

const std::string& Technique::GetTechName() const noexcept
{
	return m_Name;
}
