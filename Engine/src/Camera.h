#pragma once
#include <DirectXMath.h>
class Camera
{
public:
	Camera();
	DirectX::XMMATRIX GetMatrix() const;
	void SpwanControlWindow();
private:
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_rot;
};

