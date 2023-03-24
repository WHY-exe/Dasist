#pragma once
#include "Camera.h"
#include "Graphics.h"
#include <vector>
#include <memory>
class CameraContainer
{
public:
	CameraContainer(Graphics& gfx);
	Camera& GetCamera() noexcept;
	void UpdateDefaultValues(Graphics& gfx) noexcept(!IS_DEBUG);
	void Bind(Graphics& gfx) noexcept(!IS_DEBUG);
	void SpawControlWindow() noexcept(!IS_DEBUG);
	void Add(std::unique_ptr<Camera>& camera) noexcept(!IS_DEBUG);
	void Add(float ViewWidth, float ViewHeight) noexcept(!IS_DEBUG);
	void Add(Graphics& gfx) noexcept(!IS_DEBUG);
private:
	unsigned int m_cur_idx = 0;
	float m_defaultViewWidth, m_defaultViewHeight;
	std::vector<std::unique_ptr<Camera>> m_Container;
};

