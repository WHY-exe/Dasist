#pragma once
#include "Bindable.h"
#include "IndexBuffer.h"
#include <map>
#include <memory>
#include <DirectXMath.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Probe.h"
#include "Topology.h"
#include "FrameCommander.h"
#include "Technique.h"
class Drawable
{
	friend class DrawableBase;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	void AddTechnique(Technique& tech) noexcept;
	void AddEssentialBind(std::shared_ptr<Bindable> bind) noexcept;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	virtual ~Drawable() = default;
	void Bind(Graphics& gfx) const noexcept;
	void Accept(Probe& probe) noexcept;
	void Submit(FrameCommander& frameCommander) const noexcept;
	UINT GetIndexSize() const noexcept;
private:
	const IndexBuffer* m_pIndexBuffer = nullptr;
	std::vector<std::shared_ptr<Bindable>> m_essential_binds;
	std::vector<Technique> m_Techs;
};

