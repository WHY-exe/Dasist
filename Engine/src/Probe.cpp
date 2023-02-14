#include "Probe.h"

void Probe::SetTechnique(Technique* tech_in)
{	
	m_pTech = tech_in;
	OnSetTechnique();
}

void Probe::SetStep(Step* step_in)
{	
	m_pStep = step_in;
	OnSetStep();
}
