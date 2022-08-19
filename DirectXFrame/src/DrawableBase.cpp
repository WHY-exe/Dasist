#include "DrawableBase.h"

bool DrawableBase::IsStaticInitialized(const std::string& szBufferName) const noexcept
{
	return m_staticBind.find(szBufferName) != m_staticBind.end();
}

void DrawableBase::InitStaticSlot(const std::string& szBufferName) const noexcept
{
	m_staticBind.insert({ szBufferName, std::vector<std::unique_ptr<Bindable>>() });
}

bool DrawableBase::IsSlotInitialized(const std::string& szBufferName) const noexcept
{
	return !m_staticBind.find(szBufferName)->second.empty();
}

void DrawableBase::AddStaticBind(const std::string& szBufferName, std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG)
{
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	m_staticBind.find(szBufferName)->second.push_back(std::move(bind));
}

void DrawableBase::AddStaticIndexBuffer(const std::string& szBufferName, std::unique_ptr<IndexBuffer> idx_buf) noexcept(!IS_DEBUG)
{
	assert("Attempting to add index buffer a second time" && m_pIndexBuffer == nullptr);
	m_pIndexBuffer = idx_buf.get();
	m_staticBind.find(szBufferName)->second.push_back(std::move(idx_buf));
}

void DrawableBase::SetIndexfromStatic(const std::string& szBufferName) noexcept(!IS_DEBUG)
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

const std::map<std::string, std::vector<std::unique_ptr<Bindable>>>& DrawableBase::GetStaticBinds() const noexcept
{
	return m_staticBind;
}

std::map<std::string, std::vector<std::unique_ptr<Bindable>>> DrawableBase::m_staticBind;