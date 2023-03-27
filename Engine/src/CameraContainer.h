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
	void Bind(Graphics& gfx) noexcept(!IS_DEBUG);
	void SpawControlWindow() noexcept(!IS_DEBUG);
	void DeleteCurCamera() noexcept(!IS_DEBUG);
	void Add(std::unique_ptr<Camera>& camera) noexcept(!IS_DEBUG);
	void Add(Graphics& gfx) noexcept(!IS_DEBUG);
	void LinkTechniques(Rgph::RenderGraph& rg);
	void LinkAddedCamera(Rgph::RenderGraph& rg);
	void Submit() const;
	bool signalCamAdded = false;
private:
	unsigned int m_cur_idx = 0u;
	unsigned int m_cur_selected_cam = 0;
	unsigned int m_lifeTimeSize = 1u;
	Graphics& m_gfx;
	std::vector<std::unique_ptr<Camera>> m_Container;
};

