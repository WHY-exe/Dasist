#pragma once
#include "Step.h"
#include "FrameCommander.h"
#include <vector>
class Technique
{
public:
	void Submit(FrameCommander& fc, const class Drawable& d) const noexcept;
	void InitializeParentReference(const class Drawable& d) noexcept;
	void AddStep(Step& step) noexcept;
	void Activate() noexcept;
	void Deactivate() noexcept;
private:
	bool m_activate = true;
	std::vector<Step> m_Steps;
};
