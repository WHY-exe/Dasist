#include "Pass.h"

void Pass::Accept(Job&& job) noexcept
{
	m_Jobs.push_back(job);
}

void Pass::Accept(const Job& job) noexcept
{
	m_Jobs.push_back(job);
}

void Pass::Excecute(Graphics& gfx) const noexcept
{
	if (!m_Jobs.empty())
	{	
		for (auto& j : m_Jobs)
		{
			j.Excute(gfx);
		}
	}

}

void Pass::Reset() noexcept
{
	m_Jobs.clear();
}
