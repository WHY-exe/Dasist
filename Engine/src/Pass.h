#pragma once
#include "Job.h"
#include <vector>
class Pass
{
public:
	void Accept(const Job& job) noexcept;
	void Excecute(Graphics& gfx) const noexcept;
	void Reset() noexcept;
private:
	std::vector<Job> m_Jobs;
};