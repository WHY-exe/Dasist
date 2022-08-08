#pragma once
#include "DrawableBase.h"
class Box : public DrawableBase<Box>
{
public:
	Box(Graphics& gfx);
	void Update(float dt) noexcept override;
	void SetPosition(float x, float y, float z);
	void SetRotSpeed(float x_srot, float y_srot, float z_srot);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	// position
	float m_pos_x = 0.0f;
	float m_pos_y = 0.0f;
	float m_pos_z = 0.0f;
private:
	float x_rot_speed = 0.0f;
	float y_rot_speed = 0.0f;
	float z_rot_speed = 0.0f;
private:
	float z_roll = 0;
	float x_roll = 0;
	float y_roll = 0;
};

