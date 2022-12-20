#pragma once
#include "Pass.h"
#include <array>
class FrameCommander
{
public:
	void Accept(const Job& job, size_t target) const noexcept;
	void Excecute(Graphics& gfx) const noexcept(!IS_DEBUG);
	void Reset() noexcept;
private:
	mutable std::array<Pass, 3> m_Passes;
};