#include "Vertex.h"

Vertex::Layout::Element::Element(ElementType type, size_t offset)
	:
	m_type(type),
	m_offset(offset)
{}

Vertex::Layout::ElementType Vertex::Layout::Element::GetType() const noexcept
{
	return m_type;
}

size_t Vertex::Layout::Element::GetOffset() const noexcept
{
	return m_offset;
}

size_t Vertex::Layout::Element::GetOffsetAfter() const noexcept(!IS_DEBUG)
{
	return m_offset + Size();
}

size_t Vertex::Layout::Element::Size() const noexcept(!IS_DEBUG)
{
	return SizeOf(m_type);
}

constexpr size_t Vertex::Layout::Element::SizeOf(ElementType type) noexcept(!IS_DEBUG)
{
	switch (type)
	{
	case ElementType::Position2D:
		return sizeof(Map<Position2D>::SysType);
		break;
	case ElementType::Position3D:
		return sizeof(Map<Position3D>::SysType);
		break;
	case ElementType::Tex2D:
		return sizeof(Map<Tex2D>::SysType);
		break;
	case ElementType::Normal:
		return sizeof(Map<Normal>::SysType);
		break;
	case ElementType::Float3Color:
		return sizeof(Map<Float3Color>::SysType);
		break;
	case ElementType::Float4Color:
		return sizeof(Map<Float4Color>::SysType);
		break;
		return sizeof(Map<Normal>::SysType);
	case ElementType::Tangent:
		return sizeof(Map<Tangent>::SysType);
	case ElementType::Bitangent:
		return sizeof(Map<Bitangent>::SysType);
	case ElementType::Byte4Color:
		return sizeof(Map<Byte4Color>::SysType);
		break;
	}
	assert("Invalid Element Type" && false);
	return 0u;
}

D3D11_INPUT_ELEMENT_DESC Vertex::Layout::Element::GetLayoutDesc() const noexcept(!IS_DEBUG)
{
	switch (m_type)
	{
	case ElementType::Position2D:
		return GenDesc<Position2D>(m_offset);
		break;
	case ElementType::Position3D:
		return GenDesc<Position3D>(m_offset);
		break;
	case ElementType::Tex2D:
		return GenDesc<Tex2D>(m_offset);
		break;
	case Normal:
		return GenDesc<Normal>(m_offset);
		break;
	case ElementType::Tangent:
		return GenDesc<Tangent>(m_offset);
	case ElementType::Bitangent:
		return GenDesc<Bitangent>(m_offset);
	case ElementType::Float3Color:
		return GenDesc<Float3Color>(m_offset);
		break;
	case ElementType::Float4Color:
		return GenDesc<Float4Color>(m_offset);
		break;
	case Byte4Color:
		return GenDesc<Byte4Color>(m_offset);
		break;
	}
	assert("Invalid element type" && false);
	return { "INVALID", 0u, DXGI_FORMAT_UNKNOWN, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u };
}

const char* Vertex::Layout::Element::GetCode() const noexcept
{
	switch (m_type)
	{
	case Position2D:
		return Map<Position2D>::code;
	case Position3D:
		return Map<Position3D>::code;
	case Tex2D:
		return Map<Tex2D>::code;
	case Normal:
		return Map<Normal>::code;
	case Float3Color:
		return Map<Float3Color>::code;
	case ElementType::Tangent:
		return Map<Tangent>::code;
	case ElementType::Bitangent:
		return Map<Bitangent>::code;
	case Float4Color:
		return Map<Float4Color>::code;
	case Byte4Color:
		return Map<Byte4Color>::code;
	}
	assert("Invalid element type" && false);
	return "Invalid";
}

const Vertex::Layout::Element& Vertex::Layout::Resolve(ElementType Type) const noexcept(!IS_DEBUG)
{
	for (const auto& e : m_Elements)
	{
		if (e.GetType() == Type)
		{
			return e;
		}
	}
	assert("Can't resolve element type" && false);
	return m_Elements.front();
}

const Vertex::Layout::Element& Vertex::Layout::ResolveByIndex(size_t i) const noexcept(!IS_DEBUG)
{
	return m_Elements[i];
}

Vertex::Layout& Vertex::Layout::Append(ElementType Type) noexcept(!IS_DEBUG)
{
	Element e(Type, Size());
	m_d3d_desc.emplace_back(e.GetLayoutDesc());
	m_Elements.emplace_back(std::move(e));
	return *this;
}

size_t Vertex::Layout::Size() const noexcept(!IS_DEBUG)
{
	return m_Elements.empty() ? 0u : m_Elements.back().GetOffsetAfter();
}

const std::vector<D3D11_INPUT_ELEMENT_DESC>& Vertex::Layout::GetD3DLayout() const
{
	return m_d3d_desc;
}

std::string Vertex::Layout::GetCode() const noexcept
{
	std::string code;
	for (auto e : m_Elements)
	{
		code += e.GetCode();
	}
	return code;
}

Vertex::Data::Data(char* pBuffer, const Layout& layout) noexcept(!IS_DEBUG)
	:
	m_pBuffer(pBuffer),
	m_layout(layout)
{
	assert(m_pBuffer != nullptr);
}

Vertex::ConstData::ConstData(const Data& data) noexcept(!IS_DEBUG)
	:
	m_data(data)
{
}

Vertex::DataBuffer::DataBuffer(const Layout& layout)
	:
	m_layout(layout)
{}

const Vertex::Layout& Vertex::DataBuffer::GetLayout() const noexcept
{
	return m_layout;
}

const char* Vertex::DataBuffer::GetData() const noexcept
{
	return m_Buffer.data();
}

size_t Vertex::DataBuffer::ByteWidth() const noexcept
{
	return m_Buffer.size();
}

size_t Vertex::DataBuffer::Size() const noexcept
{
	return m_Buffer.size() / m_layout.Size();
}

Vertex::Data Vertex::DataBuffer::Back()
{
	assert(m_Buffer.size() != 0u);
	return Vertex::Data(m_Buffer.data() + m_Buffer.size() - m_layout.Size(), m_layout);
}

Vertex::ConstData Vertex::DataBuffer::Back() const noexcept(!IS_DEBUG)
{
	return const_cast<DataBuffer*>(this)->Back();
}

Vertex::Data Vertex::DataBuffer::Front()
{
	assert(m_Buffer.size() != 0u);
	return Data(m_Buffer.data(), m_layout);
}

Vertex::ConstData Vertex::DataBuffer::Front() const noexcept(!IS_DEBUG)
{
	return const_cast<DataBuffer*>(this)->Front();
}

Vertex::Data Vertex::DataBuffer::operator[](size_t i)
{
	assert(i < Size());
	return Data(m_Buffer.data() + m_layout.Size() * i, m_layout);
}

Vertex::ConstData Vertex::DataBuffer::operator[](size_t i) const noexcept(!IS_DEBUG)
{
	return const_cast<DataBuffer&>(*this)[i];
}