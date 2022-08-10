#pragma once
#include "DrawableBase.h"
class Box : public DrawableBase<Box>
{
public:
	Box(Graphics& gfx, UINT box_id);
	void Update(float dt) noexcept override;
	void SpwanControlWindow();
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	UINT m_box_id;
private:
	// position
	float m_pos_x = 0.0f;
	float m_pos_y = 0.0f;
	float m_pos_z = 0.0f;
private:
	float z_roll = 0;
	float x_roll = 0;
	float y_roll = 0;
};

