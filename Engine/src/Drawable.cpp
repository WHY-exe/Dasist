#include "Drawable.h"

void Drawable::Draw(Graphics& gfx)
{
	for (auto& b: m_binds)
	{
		b->Bind(gfx);
	}
	for (auto& b: m_staticBind.find(GetModelName())->second)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(m_pIndexBuffer->GetSize());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	m_binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
{
	assert("Attempting to add index buffer a second time" && m_pIndexBuffer == nullptr);
	m_pIndexBuffer = ibuf.get();
	m_binds.push_back(std::move(ibuf));
}

bool Drawable::IsStaticInitialized(const std::string& szBufferName) const noexcept
{
	return m_staticBind.find(szBufferName) != m_staticBind.end();
}

void Drawable::InitStaticSlot(const std::string& szBufferName) const noexcept
{
	m_staticBind.insert({ szBufferName, std::vector<std::unique_ptr<Bindable>>() });
}

bool Drawable::IsSlotInitialized(const std::string& szBufferName) const noexcept
{
	return !m_staticBind.find(szBufferName)->second.empty();
}

void Drawable::AddStaticBind(const std::string& szBufferName, std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG)
{
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	m_staticBind.find(szBufferName)->second.push_back(std::move(bind));
}

void Drawable::AddStaticIndexBuffer(const std::string& szBufferName, std::unique_ptr<IndexBuffer> idx_buf) noexcept(!IS_DEBUG)
{
	assert("Attempting to add index buffer a second time" && m_pIndexBuffer == nullptr);
	m_pIndexBuffer = idx_buf.get();
	m_staticBind.find(szBufferName)->second.push_back(std::move(idx_buf));
}

void Drawable::SetIndexfromStatic(const std::string& szBufferName) noexcept(!IS_DEBUG)
{
	assert(m_pIndexBuffer == nullptr);
	auto it = m_staticBind.find(szBufferName);
	for (auto& b : it->second)
	{
		if (const auto& p = dynamic_cast<IndexBuffer*>(b.get()))
		{
			m_pIndexBuffer = p;
			return;
		}
	}
	assert("fail to find index buffer from static binds" && m_pIndexBuffer != nullptr);
}

std::map<std::string, std::vector<std::unique_ptr<Bindable>>> Drawable::m_staticBind;