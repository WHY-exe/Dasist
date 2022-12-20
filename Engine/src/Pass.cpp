#include "Pass.h"

void Pass::Accept(const Job& job) noexcept
{
	m_Jobs.push_back(std::move(job));
}

void Pass::Excecute(Graphics& gfx) const noexcept
{
	for (auto& j : m_Jobs)
	{
		j.Excute(gfx);
	}
}

void Pass::Reset() noexcept
{
	m_Jobs.clear();
}
