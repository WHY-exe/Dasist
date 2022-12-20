#pragma once
#include "Bindable.h"
#include "IndexBuffer.h"
#include <map>
#include <memory>
#include <DirectXMath.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
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
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	virtual ~Drawable() = default;
	void Bind(Graphics& gfx) const noexcept;
	void Submit(FrameCommander& frameCommander) const noexcept;
	UINT GetIndexSize() const noexcept;
private:
	std::shared_ptr<VertexBuffer> m_pVertexBuffer;
	std::shared_ptr<IndexBuffer> m_pIndexBuffer;
	std::shared_ptr<Topology> m_pTopology;	
	std::vector<Technique> m_Techs;
};

