#pragma once
#include <DirectXMath.h>
class Camera
{
public:
	Camera();
	DirectX::XMMATRIX GetMatrix() const;
	void Rotate(float dx, float dy) noexcept;
	void Translate(float dx, float dy, float dz);
	bool MouseStatus() const noexcept;
	void SpwanControlWindow();
	void HideMouse() noexcept;
	void ShowMouse() noexcept;
private:
	DirectX::XMMATRIX GetFPMatrix() const;
	DirectX::XMMATRIX GetTPMatrix() const;
private:
	bool m_bView = true;
	bool m_hideMouse = false;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_rot;
	static constexpr float m_rot_speed = 0.004f;
};

