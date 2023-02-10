#pragma once
namespace DCBuf {
	class Buffer;
}
class TechProbe
{
public:
	void SetTechnique(class Technique* tech_in);
	void SetStep(class Step* step_in);
	virtual bool VisitBuffer(class DCBuf::Buffer) = 0;
protected:
	virtual void OnSetTechnique() {};
	virtual void OnSetStep() {};
private:
	class Technique* m_pTech;
	class Step* m_pStep;
};

