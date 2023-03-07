#pragma once
#include "Graphics.h"
#include "GraphicsResource.h"
#include "StrTransf.h"
#include "Probe.h"
#include <memory>
#include <string>
class Bindable : public GraphicsResource
{
public:
	virtual void Bind(Graphics& gfx) noexcept = 0;
	virtual ~Bindable() = default;
	virtual void InitializeParentReference(const class Drawable&) noexcept
	{}
	virtual void Accept(Probe& probe) noexcept {};
	virtual std::wstring GetUID() const noexcept
	{
		return L"";
	}
};

