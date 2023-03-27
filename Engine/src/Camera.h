#pragma once
#include <DirectXMath.h>
#include "CameraIndicator.h"
#include "FOVIndicator.h"
#include <string>
namespace Rgph
{
	class RenderGraph;
}
class Camera
{
public:
	Camera(class Graphics& gfx, std::string szName);
	DirectX::XMMATRIX GetCameraMatrix() const;
	DirectX::XMMATRIX GetPerspectiveViewMX() const noexcept(!IS_DEBUG);
	const std::string& GetName() const noexcept;
	void Rotate(float dx, float dy) noexcept;
	void Translate(float dx, float dy, float dz);
	bool MouseStatus() const noexcept;
	void UpdateDefaultValues(class Graphics& gfx) noexcept;
	void ShowControlWidget() noexcept(!IS_DEBUG);
	void HideMouse() noexcept;
	void ShowMouse() noexcept;
	void LinkTechniques(Rgph::RenderGraph& rg);
	void Submit() const;
private:
	DirectX::XMMATRIX GetFPMatrix() const;
	DirectX::XMMATRIX GetTPMatrix() const;
private:
	Graphics& m_gfx;
	std::string m_szName;
	bool m_hideMouse = false;
	unsigned int m_defaultViewWidth;
	unsigned int m_defaultViewHeight;
	float m_viewWidth;
	float m_viewHeight;
	float m_NearZ;
	float m_FarZ;	
	FOVIndicator m_fov_indi;
	CameraIndicator m_indicator;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_rot;
	static constexpr float m_rot_speed = 0.004f;
	static constexpr DirectX::XMFLOAT3 homePos = { 0.0f, 100.0f, -25.0f };
};

