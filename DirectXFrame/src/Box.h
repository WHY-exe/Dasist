#pragma once
#include "Drawable.h"
class Box : public Drawable
{
public:
	Box(Graphics& gfx);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	float r_roll_speed = 0;
	float x_roll_speed = 0;
};

