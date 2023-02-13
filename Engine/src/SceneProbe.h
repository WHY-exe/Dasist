#pragma once
#include "TechProbe.h"

namespace Scene {
	class Probe: public TechProbe
	{
	public:
		bool VisitBuffer(class DCBuf::Buffer material_data) override;
	protected:
		void OnSetTechnique() override;
		void OnSetStep() override;
	};
}

