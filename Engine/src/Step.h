#pragma once
#include "Bindable.h"
#include "Probe.h"
#include <memory>
class Step
{
public:
	Step(size_t pass_index);
	void AddBind(std::shared_ptr<Bindable> bind) noexcept;
	void SetActiveState(bool active) noexcept;
	bool IsActive() const noexcept;
	void InitializeParentReference(const class Drawable& d) noexcept;
	void Submit(const class FrameCommander& fc, const class Drawable& d) const;
	void Bind(Graphics& gfx) const;
	void Accept(Probe& probe) noexcept;
private:
	bool m_active_state = true;
	size_t m_pass_index;
	std::vector<std::shared_ptr<Bindable>> m_Binds;
};

