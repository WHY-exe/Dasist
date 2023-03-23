#pragma once
#include "DynamicConstantBuffer.h"
#include <string>
#include <memory>
#include <unordered_map>

namespace DCBuf
{
	class LayoutCodeX
	{
	public:
		static CookedLayout Resolve(RawLayout&& layout) noexcept(!_DEBUG);
	private:
		static LayoutCodeX& Get_() noexcept;
		std::unordered_map<std::string, std::shared_ptr<LayoutElement>> m_elTable;
	};
}

