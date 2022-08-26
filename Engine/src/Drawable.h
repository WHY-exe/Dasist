#pragma once
#include "Bindable.h"
#include "IndexBuffer.h"
#include <map>
#include <memory>
#include <DirectXMath.h>
class Drawable
{
	friend class DrawableBase;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	void Draw(Graphics& gfx);
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	virtual const std::string& GetModelName() const noexcept = 0;
	virtual void SpwanControlWindow() noexcept = 0;
	virtual void Update(Graphics& gfx) noexcept = 0;
	virtual ~Drawable() = default;
protected:
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept;
protected:
	bool IsStaticInitialized(const std::string& szBufferName) const noexcept;
	void InitStaticSlot(const std::string& szBufferName) const noexcept;
	bool IsSlotInitialized(const std::string& szBufferName) const noexcept;
	void AddStaticBind(const std::string& szBufferName, std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG);
	void AddStaticIndexBuffer(const std::string& szBufferName, std::unique_ptr<IndexBuffer> idx_buf) noexcept(!IS_DEBUG);
	void SetIndexfromStatic(const std::string& szBufferName) noexcept(!IS_DEBUG);
private:
	static std::map<std::string, std::vector<std::unique_ptr<Bindable>>> m_staticBind;	
	const IndexBuffer* m_pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> m_binds;
};

