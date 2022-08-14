#pragma once
#include "ModelObj.h"
class Box : public ModelObj<Box>
{
public:
	Box(Graphics& gfx, UINT box_id);
	void SpwanControlWindow() noexcept override;
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	UINT m_box_id;
	static const Model m_obj;
};
