#pragma once
#include "Drawable.h"
#include <map>
class DrawableBase : public Drawable
{
protected:
	bool IsStaticInitialized(const std::string& szBufferName) const noexcept;
	void InitStaticSlot(const std::string& szBufferName) const noexcept;
	bool IsSlotInitialized(const std::string& szBufferName) const noexcept;
	void AddStaticBind(const std::string& szBufferName, std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG);
	void AddStaticIndexBuffer(const std::string& szBufferName, std::unique_ptr<IndexBuffer> idx_buf) noexcept(!IS_DEBUG);
	void SetIndexfromStatic(const std::string& szBufferName) noexcept(!IS_DEBUG);
	virtual const std::map<std::string, std::vector<std::unique_ptr<Bindable>>>& GetStaticBinds() const noexcept override;
private:
	static std::map<std::string, std::vector<std::unique_ptr<Bindable>>> m_staticBind;
};
