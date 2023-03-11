#pragma once
#include "Step.h"
#include "Probe.h"
#include "FrameCommander.h"
#include <vector>
class Technique
{
public:
	Technique(const std::string& name) noexcept;
	void Submit(FrameCommander& fc, const class Drawable& d) const noexcept;
	void InitializeParentReference(const class Drawable& d) noexcept;
	void AddStep(Step& step) noexcept;
	void SetStepActive(UINT step_index, bool active) noexcept;
	void SetActiveState(bool is_active) noexcept;
	bool IsActive() const noexcept;
	void Accept(Probe& probe) noexcept;
	const std::string& GetTechName() const noexcept;
private:
	std::string m_Name;
	bool m_activate = true;
	std::vector<Step> m_Steps;
};
