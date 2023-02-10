#include "TechProbe.h"

void TechProbe::SetTechnique(Technique* tech_in)
{	
	m_pTech = tech_in;
	OnSetTechnique();
}

void TechProbe::SetStep(Step* step_in)
{	
	m_pStep = step_in;
	OnSetStep();
}
