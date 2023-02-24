#include "Job.h"
#include "Step.h"
#include "Drawable.h"
Job::Job(const Step* pStep, const Drawable* pDrawable)
	:
	m_pStep(pStep),
	m_pDrawable(pDrawable)
{
}

void Job::Excute(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	if (m_pStep->IsActive())
	{
		m_pDrawable->Bind(gfx);
		m_pStep->Bind(gfx);
		gfx.DrawIndexed(m_pDrawable->GetIndexSize());
	}
}
