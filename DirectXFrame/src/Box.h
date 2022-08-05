#pragma once
#include "DrawableBase.h"
class Box : public DrawableBase<Box>
{
public:
	Box(Graphics& gfx);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	float m_pos_x = 0.0f;
	float m_pos_y = 0.0f;
	float m_pos_z = 4.0f;
private:
	float x_rot_speed = 1.0f;
	float y_rot_speed = 2.0f;
	float z_rot_speed = 3.0f;
private:
	float z_roll;
	float x_roll;
	float y_roll;
};

